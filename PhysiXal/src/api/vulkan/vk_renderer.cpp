#include "px_pch.h"
#include "api/vulkan/vk_renderer.h"

#include "api/vulkan/vk_context.h"
#include "api/vulkan/vk_device.h"
#include "api/vulkan/vk_swap_chain.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	struct GLFWwindow* windowHandle;

	static VulkanContext* m_Context = nullptr;

	static VulkanContext* InitContext()
	{
		return new VulkanContext();
	}

	static VulkanDevice* m_Device = nullptr;

	static VulkanDevice* InitDevice()
	{
		return new VulkanDevice();
	}

	static VulkanSwapChain* m_SwapChain = nullptr;

	static VulkanSwapChain* InitSwapChain()
	{
		return new VulkanSwapChain();
	}

	void VulkanRenderer::Init()
	{
		PX_CORE_INFO("Initializing the renderer");
		
		m_Context = InitContext();
		m_Device = InitDevice();
		m_SwapChain = InitSwapChain();

		m_Context->CreateContext();
		m_Context->SetupDebugMessenger();
		m_Device->CreateSurface();
		m_Device->PickPhysicalDevice();
		m_Device->CreateLogicalDevice();
		m_Device->CreateSwapChain();
	}
	
	void VulkanRenderer::Shutdown()
	{
		m_Device->DestroySwapChain();
		m_Device->DestroyDevice();
		m_Device->DestroySurface();
		m_Context->DestroyContext();
		PX_CORE_WARN("...Shutting down the renderer");
	}

	void VulkanRenderer::BeginFrame()
	{
	}
#endif
}