#include "px_pch.h"
#include "api/vulkan/vk_context.h"

#include "api/vulkan/vk_utilities.h"
#include "api/vulkan/vk_initializers.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Context (instance)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void VulkanContext::CreateContext()
	{
		PX_CORE_INFO("Creating Vulkan context");

		PX_CORE_ASSERT(glfwVulkanSupported(), "GLFW must support Vulkan!");

		// Application info
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "PhysiXal";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "PhysiXal";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		// Extensions and validation
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (s_EnableValidation)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
			createInfo.ppEnabledLayerNames = ValidationLayers.data();

			PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else 
		{
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}

		// Instance creation
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

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Debug messenger
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void VulkanContext::SetupDebugMessenger()
	{
		PX_CORE_INFO("Seting up Vulkan Debug messenger");

		if (!s_EnableValidation) return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(s_VulkanInstance, &createInfo, nullptr, &s_DebugMessenger) != VK_SUCCESS)
		{
			PX_CORE_ERROR("Failed to set up debug messenger!");
		}
	}

	void VulkanContext::DestroyDebugMessenger()
	{
		PX_CORE_WARN("...Destroying Vulkan Debug messenger");

		if (s_EnableValidation)
		{
			DestroyDebugUtilsMessengerEXT(s_VulkanInstance, s_DebugMessenger, nullptr);
		}
	}

	void VulkanContext::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) 
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Extensions and validation layers
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::vector<const char*> VulkanContext::GetRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (s_EnableValidation)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool VulkanContext::CheckValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : ValidationLayers) 
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) 
			{
				if (strcmp(layerName, layerProperties.layerName) == 0) 
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound) 
			{
				return false;
			}
		}

		return true;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanContext::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		PX_CORE_TRACE("Validation layer: {0}", pCallbackData->pMessage);

		return VK_FALSE;
	}
}
