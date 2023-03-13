#include "px_pch.h"
#include "api/vulkan/vk_context.h"

#include "api/vulkan/vk_utilities.h"
#include "api/vulkan/vk_initializers.h"

#include "core/application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Context (instance)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void VulkanContext::CreateContext()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_INFO("Creating Vulkan context");

		PX_CORE_ASSERT(glfwVulkanSupported(), "GLFW must support Vulkan!");

		// Vulkan loves structs.  I mean, really loves structs.
		// Most of Vulkan code is simply filling out structs
		// and submitting that to an API call with the necessary
		// object handles attached
		// 
		// #### Application info ####
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "PhysiXal";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "PhysiXal";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		// Enables Vulkan validation layers. ( Debug information )
		// 
		// #### Extensions and validation ####
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		// Give all the extensions/layers to the create info.
		auto extensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<U32>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (s_EnableValidation)
		{
			createInfo.enabledLayerCount = static_cast<U32>(ValidationLayers.size());
			createInfo.ppEnabledLayerNames = ValidationLayers.data();

			PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else 
		{
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}

		// Make the API call
		// 
		// #### Instance creation ####
		if (vkCreateInstance(&createInfo, nullptr, &s_VulkanInstance) != VK_SUCCESS)
		{
			PX_CORE_ERROR("Failed to create instance!");
		}
	}
		
	void VulkanContext::DestroyContext()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_WARN("...Destroying Vulkan context");

		vkDestroyInstance(s_VulkanInstance, nullptr);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  Surface
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void VulkanContext::CreateSurface()
	{
		PX_PROFILE_FUNCTION();

		// We create a multi-platform specific presentation surface.
		// 
		// This is the window's application handle.
		auto vkWindowHandle = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		PX_CORE_INFO("Creating Vulkan surface");

		// Vulkan's freshly created instance gets feed into the API call.
		// Vulkan follows this idiom to a fault.
		if (glfwCreateWindowSurface(VulkanContext::GetVulkanInstance(), vkWindowHandle, nullptr, &s_Surface) != VK_SUCCESS)
		{
			PX_CORE_ERROR("Failed to create window surface!");
		}
	}

	void VulkanContext::DestroySurface()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_WARN("...Destroying Vulkan surface");

		vkDestroySurfaceKHR(VulkanContext::GetVulkanInstance(), s_Surface, nullptr);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Debug messenger
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void VulkanContext::SetupDebugMessenger()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_INFO("Seting up Vulkan debug messenger");

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
		PX_PROFILE_FUNCTION();

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
		U32 glfwExtensionCount = 0;
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
		U32 layerCount;
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
