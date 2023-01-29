#pragma once

#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace PhysiXal {

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
		// Create context (instance)
		void CreateContext();
		void DestroyContext();

		static VkInstance GetVulkanInstance() { return s_VulkanInstance; }
		
		// Debug messenger
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void SetupDebugMessenger();
		
		// Extensions and validation layers
		std::vector<const char*> GetRequiredExtensions();
		bool CheckValidationLayerSupport();
	private:
		inline static VkInstance s_VulkanInstance;

		inline static VkDebugUtilsMessengerEXT s_DebugMessenger;
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
			VkDebugUtilsMessageTypeFlagsEXT messageType, 
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
	};
}