#include "px_pch.h"
#include "api/vulkan/vk_renderer.h"

#include "api/vulkan/vk_context.h"
#include "api/vulkan/vk_device.h"
#include "api/vulkan/vk_pipeline.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	struct GLFWwindow* windowHandle;

	static VulkanContext* m_Context = nullptr;
	static VulkanDevice* m_Device = nullptr;
	static VulkanPipeline* m_Pipeline = nullptr;

	static VulkanContext* InitContext()
	{
		return new VulkanContext();
	}

	static VulkanDevice* InitDevice()
	{
		return new VulkanDevice();
	}

	static VulkanPipeline* InitPipeline()
	{
		return new VulkanPipeline();
	}

	void VulkanRenderer::Init()
	{
		PX_CORE_INFO("Initializing the renderer");
		
		m_Context = InitContext();
		m_Device = InitDevice();
		m_Pipeline = InitPipeline();


		m_Context->CreateContext();
		m_Context->SetupDebugMessenger();
		m_Device->CreateSurface();
		m_Device->PickPhysicalDevice();
		m_Device->CreateLogicalDevice();
		m_Device->CreateSwapChain();
		m_Device->CreateImageViews();
		m_Pipeline->CreateRenderPass();
		m_Pipeline->InitGraphicsPipeline();
	}
	
	void VulkanRenderer::Shutdown()
	{
		PX_CORE_WARN("...Shutting down the renderer");

		m_Pipeline->ShutdownGraphicsPipeline();
		m_Pipeline->DestroyRenderPass();
		m_Device->DestroyImageViews();
		m_Device->DestroySwapChain();
		m_Device->DestroyDevice();
		m_Device->DestroySurface();
		m_Context->DestroyContext();
	}

	void VulkanRenderer::BeginFrame()
	{
	}
#endif
}