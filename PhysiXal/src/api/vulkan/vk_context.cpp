#include "px_pch.h"
#include "api/vulkan/vk_context.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>



namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	VulkanContext::VulkanContext()
	{
	}

	VulkanContext::~VulkanContext()
	{
	}

	void VulkanContext::InitContext()
	{
		PX_CORE_INFO("Creating Vulkan context");

		PX_CORE_ASSERT(glfwVulkanSupported(), "GLFW must support Vulkan!");

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Application Info
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "PhysiXal";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "PhysiXal";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Extensions and Validation
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        createInfo.enabledLayerCount = 0;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Instance and Surface Creation
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (vkCreateInstance(&createInfo, nullptr, &s_VulkanInstance) != VK_SUCCESS)
		{
			PX_CORE_ERROR("Failed to create instance!");
		}
	}
		
	void VulkanContext::DestroyContext()
	{
		PX_CORE_WARN("...Destroying Vulkan context");
		vkDestroyInstance(s_VulkanInstance, nullptr);
	}
#endif
}
