#include "px_pch.h"
#include "api/vulkan/vk_swap_chain.h"

#include "api/vulkan/vk_device.h"

#include "core/application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

    void VulkanSwapChain::CreateSwapChain()
    {
        VkPhysicalDevice vkPhysicalDevice = VulkanDevice::GetVulkanPhysicalDevice();
        VkSurfaceKHR vkSurface = VulkanDevice::GetVulkanSurface();
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_INFO("Creating Vulkan swap chain");

        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(vkPhysicalDevice);

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = vkSurface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = VulkanDevice::FindQueueFamilies(vkPhysicalDevice);
        uint32_t queueFamilyIndices[] = { indices.m_GraphicsFamily.value(), indices.m_PresentFamily.value() };

        if (indices.m_GraphicsFamily != indices.m_PresentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(vkDevice, &createInfo, nullptr, &s_SwapChain) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(vkDevice, s_SwapChain, &imageCount, nullptr);
        s_SwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(vkDevice, s_SwapChain, &imageCount, s_SwapChainImages.data());

        s_SwapChainImageFormat = surfaceFormat.format;
        s_SwapChainExtent = extent;
    }

    void VulkanSwapChain::DestroySwapChain()
    {
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_WARN("...Destroying Vulkan swap chain");

        vkDestroySwapchainKHR(vkDevice, s_SwapChain, nullptr);
    }

    SwapChainSupportDetails VulkanSwapChain::QuerySwapChainSupport(VkPhysicalDevice device)
    {
        VkSurfaceKHR vkSurface = VulkanDevice::GetVulkanSurface();

        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vkSurface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, vkSurface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, vkSurface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, vkSurface, &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, vkSurface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR VulkanSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const auto& availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR VulkanSwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        // Use VK_PRESENT_MODE_MAILBOX_KHR is faster and more energy consuming than traditional vSync
        // Or use VK_PRESENT_MODE_FIFO_KHR for traditional vSync
        for (const auto& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        auto vkWindowHandle = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            int width, height;
            glfwGetFramebufferSize(vkWindowHandle, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void VulkanSwapChain::CreateImageViews()
    {
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_INFO("Setting up and creating Vulkan image views");

        s_SwapChainImageViews.resize(s_SwapChainImages.size());

        for (size_t i = 0; i < s_SwapChainImages.size(); i++)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = s_SwapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = s_SwapChainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(vkDevice, &createInfo, nullptr, &s_SwapChainImageViews[i]) != VK_SUCCESS)
            {
                PX_CORE_ERROR("Failed to create image views!");
            }
        }
    }

    void VulkanSwapChain::DestroyImageViews()
    {
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_WARN("...Destroying Vulkan image views");

        for (auto imageView : s_SwapChainImageViews)
        {
            vkDestroyImageView(vkDevice, imageView, nullptr);
        }
    }
}