#include "px_pch.h"
#include "api/vulkan/vk_swap_chain.h"

#include "api/vulkan/vk_utilities.h"
#include "api/vulkan/vk_initializers.h"
#include "api/vulkan/vk_renderer.h"

#include "api/imgui/imgui_initializers.h"

#include "core/application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Swap chain
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanSwapChain::CreateSwapChain()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_INFO("Creating Vulkan swap chain");

        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(VulkanDevice::GetVulkanPhysicalDevice());

        // The GPU needs three things
        // 1.) Surface format as described earlier
        // 2.) Present mode. Again refer to documentation I shared
        // 3.) Surface extent is basically just the size ( width, height ) of the image
        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

        U32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        // There are only 8 potential bits that can be used here and I'm using two
        // Essentially this is what they mean
        // VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT - This is a color image I'm rendering into
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = VulkanContext::GetVulkanSurface();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        // If the graphics queue family and present family don't match
	    // a swap chain with different information is created
        QueueFamilyIndices indices = VulkanDevice::FindQueueFamilies(VulkanDevice::GetVulkanPhysicalDevice());
        U32 queueFamilyIndices[] = { indices.m_GraphicsFamily.value(), indices.m_PresentFamily.value() };

        if (indices.m_GraphicsFamily != indices.m_PresentFamily) 
        {
            // There are only two sharing modes 
            // This is set beacuse images are not exclusive to one queue
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            // Exclusive access to the images if the indices are the same
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;

        // Is Vulkan allowed to discard operations outside of the renderable space?
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        // Create the swapchain
        if (vkCreateSwapchainKHR(VulkanDevice::GetVulkanDevice(), &createInfo, nullptr, &s_SwapChain) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(VulkanDevice::GetVulkanDevice(), s_SwapChain, &imageCount, nullptr);
        s_SwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(VulkanDevice::GetVulkanDevice(), s_SwapChain, &imageCount, s_SwapChainImages.data());

        // Save swapchain details
        s_SwapChainImageFormat = surfaceFormat.format;
        s_SwapChainExtent = extent;
    }

    void VulkanSwapChain::DestroyCurrentSwapChain()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_WARN("...Destroying Vulkan swap chain");

        // Destruction of the swap chain
        vkDestroySwapchainKHR(VulkanDevice::GetVulkanDevice(), s_SwapChain, nullptr);
    }

    void VulkanSwapChain::DestroySwapChain()
    {
        PX_PROFILE_FUNCTION();

        // Destruction of depth buffer
        m_DepthBuffer->DestroyDepthResources();

        // Destruction of multisampling (MSAA)
        m_Device->DestroyColorResources();

        // Destruction of framebuffers
        m_Framebuffer->DestroyFramebuffers();

        // Destruction of image views
        DestroyImageViews();

        // Destruction of swap chain
        DestroyCurrentSwapChain();
    }

    SwapChainSupportDetails VulkanSwapChain::QuerySwapChainSupport(VkPhysicalDevice device)
    {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, VulkanContext::GetVulkanSurface(), &details.capabilities);

        U32 formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanContext::GetVulkanSurface(), &formatCount, nullptr);

        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanContext::GetVulkanSurface(), &formatCount, details.formats.data());
        }

        U32 presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanContext::GetVulkanSurface(), &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanContext::GetVulkanSurface(), &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR VulkanSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const auto& availableFormat : availableFormats)
        {
            // Favor 32 bit srgb format and srgb nonlinear colorspace
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR VulkanSwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        // Only looking for FIFO mode with later fix for MAILBOX on Nvidia GPU´s needed
        for (const auto& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_FIFO_KHR)
            {
                return availablePresentMode;
            }
        }
    }

    VkExtent2D VulkanSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        auto vkWindowHandle = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        if (capabilities.currentExtent.width != std::numeric_limits<U32>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            int width, height;
            glfwGetFramebufferSize(vkWindowHandle, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<U32>(width),
                static_cast<U32>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Image views
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanSwapChain::CreateImageViews()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_INFO("Setting up and creating Vulkan image views");
        
        // There are only 4 aspect bits
        // Most use 3 of them
        // They determine what is affected by the image operations
        s_SwapChainImageViews.resize(s_SwapChainImages.size());

        for (U32 i = 0; i < s_SwapChainImages.size(); i++)
        {
            s_SwapChainImageViews[i] = CreateImageView(s_SwapChainImages[i], s_SwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
        }
    }

    void VulkanSwapChain::DestroyImageViews()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_WARN("...Destroying Vulkan image views");

        // Destruction of image views
        for (auto imageView : s_SwapChainImageViews)
        {
            vkDestroyImageView(VulkanDevice::GetVulkanDevice(), imageView, nullptr);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Swap chain recreation
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanSwapChain::RecreateSwapChain()
    {
        auto vkWindowHandle = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        int width = 0, height = 0;
        glfwGetFramebufferSize(vkWindowHandle, &width, &height);
        while (width == 0 || height == 0)
        {
            glfwGetFramebufferSize(vkWindowHandle, &width, &height);
            glfwWaitEvents();
        }

        m_VulkanRenderer->WaitAndIdle();

        // #### Dear ImGUI ####

        // Destruction of Dear ImGUIs framebuffer
        m_GuiVulkan->DestroyGuiFramebuffers();

        // Destruction of Dear ImGUIs command buffers
        m_GuiVulkan->DestroyGuiCommandBuffers();

        // Destruction of Dear ImGUIs command pool
        //m_GuiVulkan->DestroyGuiCommandPool();

        // Destruction of Dear ImGUIs render pass
        m_GuiVulkan->DestroyGuiRenderPass();

        // #### Vulkan core ####

        // Destruction of swap chain all other dependencies that will be recreated
        DestroySwapChain();

        // After destruction we need to recreate everything and first the swap chain
        CreateSwapChain();

        // recreation of the image viwes
        CreateImageViews();

        // recreation of the Multisampling (MSAA)
        m_Device->CreateColorResources();

        // recreation of the depth buffer
        m_DepthBuffer->CreateDepthResources();

        // When everything else is in order we put it together for our new framebuffer
        m_Framebuffer->CreateFramebuffers();

        // We also need to take care of the UI
        m_Gui->GuiOnRebuild();
    }
}