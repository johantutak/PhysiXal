#include "px_pch.h"
#include "api/vulkan/vk_device.h"

#include "api/vulkan/vk_utilities.h"

#include "core/application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <set>
#include <cstdint>
#include <limits>

namespace PhysiXal {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Physical Device
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanDevice::PickPhysicalDevice() 
    {
        auto vkInstance = VulkanContext::GetVulkanInstance();

        PX_CORE_INFO("Finding suitable device (physical)");
        
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
                s_PhysicalDevice = device;
                s_MsaaSamples = GetMaxUsableSampleCount();
                break;
            }
        }

        if (s_PhysicalDevice == VK_NULL_HANDLE) 
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
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, s_Surface, &presentSupport);

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
        PX_CORE_INFO("Finding suitable device (logical)");

        QueueFamilyIndices indices = FindQueueFamilies(s_PhysicalDevice);

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
        deviceFeatures.samplerAnisotropy = VK_TRUE;

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

        if (vkCreateDevice(s_PhysicalDevice, &createInfo, nullptr, &s_LogicalDevice) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create logical device!");
        }

        vkGetDeviceQueue(s_LogicalDevice, indices.m_GraphicsFamily.value(), 0, &s_GraphicsQueue);
        vkGetDeviceQueue(s_LogicalDevice, indices.m_PresentFamily.value(), 0, &s_PresentQueue);
    }
    
    void VulkanDevice::DestroyDevice()
    {
        PX_CORE_WARN("...Unloading the device (logical)");

        vkDestroyDevice(s_LogicalDevice, nullptr);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  Surface
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanDevice::CreateSurface()
    {
        auto vkInstance = VulkanContext::GetVulkanInstance();
        auto vkWindowHandle = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        PX_CORE_INFO("Creating Vulkan surface");

        if (glfwCreateWindowSurface(vkInstance, vkWindowHandle, nullptr, &s_Surface) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create window surface!");
        }
    }

    void VulkanDevice::DestroySurface()
    {
        auto vkInstance = VulkanContext::GetVulkanInstance();

        PX_CORE_WARN("...Destroying Vulkan surface");

        vkDestroySurfaceKHR(vkInstance, s_Surface, nullptr);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  Command pool
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanDevice::CreateCommandPool()
    {
        QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(s_PhysicalDevice);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.m_GraphicsFamily.value();

        if (vkCreateCommandPool(s_LogicalDevice, &poolInfo, nullptr, &s_CommandPool) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create command pool!");
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Multisampling (MSAA)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanDevice::CreateColorResources()
    {
        VkFormat vkSwapChainImageFormat = VulkanSwapChain::GetVulkanImageFormat();
        VkExtent2D vkSwapChainExtent2D = VulkanSwapChain::GetVulkanSwapChainExtent();

        PX_CORE_INFO("Creating Vulkan color resources");

        VkFormat colorFormat = vkSwapChainImageFormat;

        CreateImage(vkSwapChainExtent2D.width, vkSwapChainExtent2D.height, 1, s_MsaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, s_ColorImage, s_ColorImageMemory);
        s_ColorImageView = CreateImageView(s_ColorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void VulkanDevice::DestroyColorResources()
    {


        PX_CORE_WARN("...Destroying Vulkan color resources");

        vkDestroyImageView(s_LogicalDevice, s_ColorImageView, nullptr);
        vkDestroyImage(s_LogicalDevice, s_ColorImage, nullptr);
        vkFreeMemory(s_LogicalDevice, s_ColorImageMemory, nullptr);
    }
}