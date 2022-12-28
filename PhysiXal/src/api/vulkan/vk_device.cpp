#include "px_pch.h"
#include "api/vulkan/vk_device.h"

#include "api/vulkan/vk_context.h"

#include "core/application.h"

#include <set>

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

        return indices.IsComplete();
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

        createInfo.enabledExtensionCount = 0;

        if (s_EnableValidation) 
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
            createInfo.ppEnabledLayerNames = ValidationLayers.data();
        }
        else 
        {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_LogicalDevice) != VK_SUCCESS) 
        {
            PX_CORE_ERROR("Failed to create logical device!");
        }

        vkGetDeviceQueue(m_LogicalDevice, indices.m_GraphicsFamily.value(), 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_LogicalDevice, indices.m_PresentFamily.value(), 0, &m_PresentQueue);
    }
    
    void VulkanDevice::DestroyDevice()
    {
        PX_CORE_WARN("...Unloading the device (logical)");

        vkDestroyDevice(m_LogicalDevice, nullptr);
    }

    void VulkanDevice::CreateSurface()
    {
        PX_CORE_INFO("Creating Vulkan surface");

        auto vkInstance = VulkanContext::GetVulkanInstance();
        auto vkWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        if (glfwCreateWindowSurface(vkInstance, vkWindow, nullptr, &m_Surface) != VK_SUCCESS)
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
#endif
}