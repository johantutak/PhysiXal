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
	static bool s_EnableValidation = true;
#else // PX_RELEASE || PX_DIST
	static bool s_EnableValidation = false;
#endif

	class VulkanContext
	{
	public:
		// Create context
		static void InitContext();
		static void DestroyContext();

		static VkInstance GetInstance() { return s_VulkanInstance; }
		
		// Create debug messenger
		static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		static void SetupDebugMessenger();
		
		// Get extensions and validation Layers
		static std::vector<const char*> GetRequiredExtensions();
		static bool CheckValidationLayerSupport();
	private: 
		// Vulkan instance
		inline static VkInstance s_VulkanInstance;

		// Debug messenger
		inline static VkDebugUtilsMessengerEXT m_DebugMessenger;
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
	};
#endif
}