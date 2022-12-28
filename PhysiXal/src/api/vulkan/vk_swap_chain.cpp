#include "px_pch.h"
#include "api/vulkan/vk_swap_chain.h"

#include "api/vulkan/vk_context.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	void VulkanSwapChain::InitSwapChain()
	{
	}

	void VulkanSwapChain::ShutdownSwapChain()
	{
	}
#endif
}