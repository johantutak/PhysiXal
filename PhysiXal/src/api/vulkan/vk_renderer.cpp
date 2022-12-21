#include "px_pch.h"
#include "api/vulkan/vk_renderer.h"

#include "api/vulkan/vk_context.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	void VulkanRenderer::Init()
	{
		PX_CORE_INFO("Initializing the renderer");
		VulkanContext::InitContext();
	}
	
	void VulkanRenderer::Shutdown()
	{
		VulkanContext::DestroyContext();
		PX_CORE_WARN("...Shutting down the renderer");
	}

	void VulkanRenderer::BeginFrame()
	{
	}
#endif
}