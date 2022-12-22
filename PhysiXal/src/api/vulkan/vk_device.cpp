#include "px_pch.h"
#include "api/vulkan/vk_device.h"

#include "api/vulkan/vk_context.h"

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

    void VulkanDevice::PickPhysicalDevice() 
    {
        PX_CORE_INFO("Finding suitable device");

        auto vkInstance = VulkanContext::GetInstance();

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

            if (indices.IsComplete()) 
            {
                break;
            }

            i++;
        }

        return indices;
    }
#endif
}