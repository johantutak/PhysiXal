#include "px_pch.h"
#include "api/vulkan/vk_device.h"

#include "api/vulkan/vk_utilities.h"
#include "api/vulkan/vk_initializers.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <set>
#include <cstdint>
#include <limits>

// #### TEMPORARY ####
#include <map>

namespace PhysiXal {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Physical Device
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanDevice::PickPhysicalDevice()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_INFO("Finding suitable device (physical)");

        // Get the number of devices
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(VulkanContext::GetVulkanInstance(), &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            PX_CORE_ERROR("Failed to find GPUs with Vulkan support!");
        }

        // Get the actual device
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(VulkanContext::GetVulkanInstance(), &deviceCount, devices.data());

        // Use an ordered map to automatically sort candidates by increasing score
        std::multimap<int, VkPhysicalDevice> candidates;

        if (deviceCount > 1)
        {
            for (const auto& device : devices)
            {
                // Pick device
                int score = RateDeviceSuitability(device);
                candidates.insert(std::make_pair(score, device));
            }

            // Check if the best candidate is suitable at all
            if (candidates.rbegin()->first > 0)
            {
                s_PhysicalDevice = candidates.rbegin()->second;
            }
            else
            {
                PX_CORE_ERROR("Failed to find a suitable GPU!");
            }
        }
        else // #### TO DO #### Better optimization for finding device and giveing priority to DISCRETE GPU
        {
            for (const auto& device : devices)
            {
                if (IsDeviceSuitable(device))
                {
                    // Pick device
                    s_PhysicalDevice = device;
                    s_MsaaSamples = GetMaxUsableSampleCount();

                    if (device == !VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                    {
                        return PX_CORE_ASSERT("Could not find discrete GPU."); // #### TO DO #### Get this message to show when discreate GPU is not found
                    }

                    break;
                }

                if (s_PhysicalDevice == VK_NULL_HANDLE)
                {
                    PX_CORE_ERROR("Failed to find a suitable GPU!");
                }
            }
        }

        
    }

    int VulkanDevice::RateDeviceSuitability(VkPhysicalDevice device)
    {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        int score = 0;

        // Discrete GPUs have a significant performance advantage
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) 
        {
            score += 1000;
        }

        // Maximum possible size of textures affects graphics quality
        score += deviceProperties.limits.maxImageDimension2D;

        // Application can't function without geometry shaders
        if (!deviceFeatures.geometryShader) 
        {
            return 0;
        }

        return score;
    }

    bool VulkanDevice::IsDeviceSuitable(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices = FindQueueFamilies(device);

        bool extensionsSupported = CheckDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionsSupported)
        {
            SwapChainSupportDetails swapChainSupport = VulkanSwapChain::QuerySwapChainSupport(device);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        return indices.IsComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
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
        // Add each family index to a list
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
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, VulkanContext::GetVulkanSurface(), &presentSupport);

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

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  Logical Device
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    void VulkanDevice::CreateLogicalDevice()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_INFO("Finding suitable device (logical)");

        QueueFamilyIndices indices = FindQueueFamilies(s_PhysicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { indices.m_GraphicsFamily.value(), indices.m_PresentFamily.value() };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) 
        {
            // Put it all together.
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        // If validation layers are enabled supply them here
        if (s_EnableValidation) 
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
            createInfo.ppEnabledLayerNames = ValidationLayers.data();
        }
        else 
        {
            createInfo.enabledLayerCount = 0;
        }

        // Create the device
        if (vkCreateDevice(s_PhysicalDevice, &createInfo, nullptr, &s_LogicalDevice) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create logical device!");
        }

        vkGetDeviceQueue(s_LogicalDevice, indices.m_GraphicsFamily.value(), 0, &s_GraphicsQueue);
        vkGetDeviceQueue(s_LogicalDevice, indices.m_PresentFamily.value(), 0, &s_PresentQueue);
    }
    
    void VulkanDevice::DestroyDevice()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_WARN("...Unloading the device (logical)");

        // Destruction of the logical device
        vkDestroyDevice(s_LogicalDevice, nullptr);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Multisampling (MSAA)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanDevice::CreateColorResources()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_INFO("Creating Vulkan color resources");

        VkFormat colorFormat = VulkanSwapChain::GetVulkanImageFormat();

        CreateImage(VulkanSwapChain::GetVulkanSwapChainExtent().width, VulkanSwapChain::GetVulkanSwapChainExtent().height, 1, s_MsaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, s_ColorImage, s_ColorImageMemory);
        s_ColorImageView = CreateImageView(s_ColorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void VulkanDevice::DestroyColorResources()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_WARN("...Destroying Vulkan color resources");

        vkDestroyImageView(s_LogicalDevice, s_ColorImageView, nullptr);
        vkDestroyImage(s_LogicalDevice, s_ColorImage, nullptr);
        vkFreeMemory(s_LogicalDevice, s_ColorImageMemory, nullptr);
    }
}