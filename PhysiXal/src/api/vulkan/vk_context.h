#pragma once

#ifdef PX_PLATFORM_WINDOWS
	#include <vulkan/vulkan.h>
#endif

struct GLFWwindow;

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	const std::vector<const char*> ValidationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

#ifdef PX_DEBUG
	const bool s_EnableValidation = true;
#else // PX_RELEASE || PX_DIST
	const bool s_EnableValidation = false;
#endif

	class VulkanContext
	{
	public:
		// Create context
		void CreateContext();
		void DestroyContext();

		static VkInstance GetVulkanInstance() { return s_VulkanInstance; }
		
		// Create debug messenger
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void SetupDebugMessenger();
		
		// Get extensions and validation Layers
		std::vector<const char*> GetRequiredExtensions();
		bool CheckValidationLayerSupport();
	private:
		// Vulkan instance
		inline static VkInstance s_VulkanInstance;

		// Debug messenger
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
	};
#endif
}