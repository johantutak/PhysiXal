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

	private: 
		// Vulkan instance
		inline static VkInstance s_VulkanInstance;
	};
#endif
}