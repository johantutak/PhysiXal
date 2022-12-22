#pragma once

#ifdef PX_PLATFORM_WINDOWS
	#include <vulkan/vulkan.h>
#endif

struct GLFWwindow;

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	class VulkanContext
	{
	public:
		VulkanContext();
		~VulkanContext();

		static void InitContext();
		static void DestroyContext();

		static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		static void SetupDebugMessenger();
		
		static std::vector<const char*> GetRequiredExtensions();
		static bool CheckValidationLayerSupport();

	private: 
		// Vulkan instance
		inline static VkInstance s_VulkanInstance;
		// Debug Messenger
		inline static VkDebugUtilsMessengerEXT m_DebugMessenger;
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
	};
#endif
}