#include "px_pch.h"
#include "debug/system_statistics.h"

#include "api/vulkan/vk_utilities.h"
#include "api/vulkan/vk_initializers.h"

#include "debug/cpu_id.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Statistics
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void SystemStatistics::PrintStats()
    {
        CPUInfo cinfo;

        VkPhysicalDevice vkPhysicalDevice = VulkanDevice::GetVulkanPhysicalDevice();
        vkGetPhysicalDeviceProperties(vkPhysicalDevice, &s_Properties);

        uint32_t vk_major = VK_VERSION_MAJOR(s_Properties.apiVersion);
        uint32_t vk_minor = VK_VERSION_MINOR(s_Properties.apiVersion);
        uint32_t vk_patch = VK_VERSION_PATCH(s_Properties.apiVersion);

        uint32_t driver_major = VK_VERSION_MAJOR(s_Properties.driverVersion);
        uint32_t driver_minor = VK_VERSION_MINOR(s_Properties.driverVersion);
        uint32_t driver_patch = VK_VERSION_PATCH(s_Properties.driverVersion);

        PX_CORE_INFO(".... Initializing devices ....!%");

        PX_CORE_TRACE(" #### DEVICE INFO (CPU) ####");
        PX_CORE_TRACE("VENDOR: {0}", cinfo.vendor());
        PX_CORE_TRACE("DEVICE (CPU): {0}", cinfo.model());
        PX_CORE_TRACE("CORES/THREADS: {0}", cinfo.cores());
        PX_CORE_TRACE("LOGICAL CORES: {0}", cinfo.logicalCpus());
        PX_CORE_TRACE("HYPER THREADED CPU: {0}", cinfo.isHyperThreaded());

        PX_CORE_TRACE("");

        PX_CORE_TRACE(" #### DEVICE INFO (GPU) ####");
        PX_CORE_TRACE("DEVICE (GPU): {0}", s_Properties.deviceName);
        PX_CORE_TRACE("TYPE: {0}", VulkanDeviceTypeToString(s_Properties.deviceType));
        PX_CORE_TRACE("VENDOR: {0}", VulkanVendorIDToString(s_Properties.vendorID));
        PX_CORE_TRACE("DEVICE ID: {0}", s_Properties.deviceID);
        PX_CORE_TRACE("API VERSION: {0}.{1}.{2}", vk_major, vk_minor, vk_patch);
        PX_CORE_TRACE("DRIVER VERSION: {0}.{1}.{2}", driver_major, driver_minor, driver_patch);

        PX_CORE_INFO("!%.... Readying devices for draw ....");
    }
}