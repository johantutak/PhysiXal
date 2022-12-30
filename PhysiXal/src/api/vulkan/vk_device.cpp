#include "px_pch.h"
#include "api/vulkan/vk_device.h"

#include "api/vulkan/vk_context.h"

#include "core/application.h"

#include <set>
#include <cstdint>
#include <limits>

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

    void VulkanDevice::PickPhysicalDevice() 
    {
        PX_CORE_INFO("Finding suitable device (physical)");

        auto vkInstance = VulkanContext::GetVulkanInstance();

        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);

        if (deviceCount == 0) 
        {
            PX_CORE_ERROR("Failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

        for (const auto& device : devices) 
        {
            if (IsDeviceSuitable(device)) 
            {
                m_PhysicalDevice = device;
                break;
            }
        }

        if (m_PhysicalDevice == VK_NULL_HANDLE)
        {
            PX_CORE_ERROR("Failed to find a suitable GPU!");
        }
    }

    bool VulkanDevice::IsDeviceSuitable(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices = FindQueueFamilies(device);

        bool extensionsSupported = CheckDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.IsComplete() && extensionsSupported && swapChainAdequate;
    }

    bool VulkanDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions) 
        {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    QueueFamilyIndices VulkanDevice::FindQueueFamilies(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) 
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
            {
                indices.m_GraphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentSupport);

            if (presentSupport) 
            {
                indices.m_PresentFamily = i;
            }

            if (indices.IsComplete()) 
            {
                break;
            }

            i++;
        }

        return indices;
    }
    
    void VulkanDevice::CreateLogicalDevice()
    {
        PX_CORE_INFO("Finding suitable device (logical)");

        QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { indices.m_GraphicsFamily.value(), indices.m_PresentFamily.value() };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) 
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (s_EnableValidation) 
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
            createInfo.ppEnabledLayerNames = ValidationLayers.data();
        }
        else 
        {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &s_LogicalDevice) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create logical device!");
        }

        vkGetDeviceQueue(s_LogicalDevice, indices.m_GraphicsFamily.value(), 0, &m_GraphicsQueue);
        vkGetDeviceQueue(s_LogicalDevice, indices.m_PresentFamily.value(), 0, &m_PresentQueue);
    }
    
    void VulkanDevice::DestroyDevice()
    {
        PX_CORE_WARN("...Unloading the device (logical)");

        vkDestroyDevice(s_LogicalDevice, nullptr);
    }

    void VulkanDevice::CreateSurface()
    {
        PX_CORE_INFO("Creating Vulkan surface");

        auto vkInstance = VulkanContext::GetVulkanInstance();
        auto vkWindowHandle = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        if (glfwCreateWindowSurface(vkInstance, vkWindowHandle, nullptr, &m_Surface) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create window surface!");
        }
    }

    void VulkanDevice::DestroySurface()
    {
        PX_CORE_WARN("...Destroying Vulkan surface");

        auto vkInstance = VulkanContext::GetVulkanInstance();

        vkDestroySurfaceKHR(vkInstance, m_Surface, nullptr);
    }

    void VulkanDevice::CreateSwapChain()
    {
        PX_CORE_INFO("Creating Vulkan swap chain");

        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_PhysicalDevice);

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
        createInfo.surface = m_Surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);
        uint32_t queueFamilyIndices[] = {indices.m_GraphicsFamily.value(), indices.m_PresentFamily.value()};

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

        if (vkCreateSwapchainKHR(s_LogicalDevice, &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(s_LogicalDevice, m_SwapChain, &imageCount, nullptr);
        m_SwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(s_LogicalDevice, m_SwapChain, &imageCount, m_SwapChainImages.data());

        s_SwapChainImageFormat = surfaceFormat.format;
        m_SwapChainExtent = extent;
    }

    void VulkanDevice::DestroySwapChain()
    {
        PX_CORE_WARN("...Destroying Vulkan swap chain");

        vkDestroySwapchainKHR(s_LogicalDevice, m_SwapChain, nullptr);
    }

    SwapChainSupportDetails VulkanDevice::QuerySwapChainSupport(VkPhysicalDevice device)
    {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, nullptr);

        if (formatCount != 0) 
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) 
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR VulkanDevice::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
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

    VkPresentModeKHR VulkanDevice::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
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

    VkExtent2D VulkanDevice::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
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

    void VulkanDevice::CreateImageViews()
    {
        PX_CORE_INFO("Setting up and creating Vulkan image views");

        m_SwapChainImageViews.resize(m_SwapChainImages.size());

        for (size_t i = 0; i < m_SwapChainImages.size(); i++) 
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = m_SwapChainImages[i];
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

            if (vkCreateImageView(s_LogicalDevice, &createInfo, nullptr, &m_SwapChainImageViews[i]) != VK_SUCCESS)
            {
                PX_CORE_ERROR("Failed to create image views!");
            }
        }
    }

    void VulkanDevice::DestroyImageViews()
    {
        PX_CORE_WARN("...Destroying Vulkan image views");

        for (auto imageView : m_SwapChainImageViews) 
        {
            vkDestroyImageView(s_LogicalDevice, imageView, nullptr);
        }
    }
#endif
}