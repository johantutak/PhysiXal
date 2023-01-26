#include "px_pch.h"
#include "api/vulkan/vk_renderer.h"

#include "api/vulkan/vk_context.h"
#include "api/vulkan/vk_device.h"
#include "api/vulkan/vk_swap_chain.h"
#include "api/vulkan/vk_pipeline.h"
#include "api/vulkan/vk_render_pass.h"
#include "api/vulkan/vk_framebuffer.h"
#include "api/vulkan/vk_command_buffer.h"
#include "api/vulkan/vk_sync_objects.h"
#include "api/vulkan/vk_vertex_buffer.h"

#include "core/application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

	struct GLFWwindow* windowHandle;

	static VulkanContext* m_Context = nullptr;
	static VulkanDevice* m_Device = nullptr;
	static VulkanSwapChain* m_SwapChain = nullptr;
	static VulkanRenderPass* m_RenderPass = nullptr;
	static VulkanPipeline* m_Pipeline = nullptr;
	static VulkanFramebuffer* m_Framebuffer = nullptr;
	static VulkanCommandBuffer* m_CommandBuffer = nullptr;
	static VulkanSyncObjects* m_SyncObjects = nullptr;
	static VulkanVertexBuffer* m_VertexBuffer = nullptr;

	void VulkanRenderer::Init()
	{
		PX_CORE_INFO("Initializing the renderer");

		m_Context->CreateContext();
		m_Context->SetupDebugMessenger();
		m_Device->CreateSurface();
		m_Device->PickPhysicalDevice();
		m_Device->CreateLogicalDevice();
		m_SwapChain->CreateSwapChain();
		m_SwapChain->CreateImageViews();
		m_RenderPass->CreateRenderPass();
		m_Pipeline->CreateGraphicsPipeline();
		m_Framebuffer->CreateFramebuffers();
		m_Device->CreateCommandPool();
		m_VertexBuffer->CreateVertexBuffer();
		m_CommandBuffer->CreateCommandBuffers();
		m_SyncObjects->CreateSyncObjects();
	}
	
	void VulkanRenderer::Shutdown()
	{
		PX_CORE_WARN("...Shutting down the renderer");

		DestroyRecreatedSwapChain();
		
		m_Pipeline->DestroyGraphicsPipeline();
		m_RenderPass->DestroyRenderPass();
		m_VertexBuffer->DestroyVertexBuffer();
		m_SyncObjects->DestroySyncObjects();
		m_CommandBuffer->DestroyCommandBuffers();
		m_Device->DestroyDevice();
		m_Device->DestroySurface();
		m_Context->DestroyContext();
	}

	void VulkanRenderer::BeginFrame()
	{
		PX_CORE_INFO("Drawing frame!");
	
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
		std::vector<VkFence> vkInFlightFences = VulkanSyncObjects::GetVulkanFences();
		VkSwapchainKHR vkSwapChain = VulkanSwapChain::GetVulkanSwapChain();
		std::vector<VkSemaphore> vkImageSemaphores = VulkanSyncObjects::GetVulkanImageSemaphores();
		std::vector<VkCommandBuffer> vkCommandBuffers = VulkanCommandBuffer::GetVulkanCommandBuffers();
		std::vector<VkSemaphore> vkRenderSemaphores = VulkanSyncObjects::GetVulkanRenderSemaphores();
		VkQueue vkGraphicsQueue = VulkanDevice::GetVulkanGraphicsQueue();
		VkQueue vkPresentQueue = VulkanDevice::GetVulkanPresentQueue();

		vkWaitForFences(vkDevice, 1, &vkInFlightFences[s_CurrentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;

		VkResult result = vkAcquireNextImageKHR(vkDevice, vkSwapChain, UINT64_MAX, vkImageSemaphores[s_CurrentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) 
		{
			RecreateSwapChain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) 
		{
			PX_CORE_ERROR("Failed to acquire swap chain image!");
		}

		vkResetFences(vkDevice, 1, &vkInFlightFences[s_CurrentFrame]);
		vkResetCommandBuffer(vkCommandBuffers[s_CurrentFrame], /*VkCommandBufferResetFlagBits*/ 0);
		m_CommandBuffer->RecordCommandBuffers(vkCommandBuffers[s_CurrentFrame], imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { vkImageSemaphores[s_CurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &vkCommandBuffers[s_CurrentFrame];

		VkSemaphore signalSemaphores[] = { vkRenderSemaphores[s_CurrentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(vkGraphicsQueue, 1, &submitInfo, vkInFlightFences[s_CurrentFrame]) != VK_SUCCESS)
		{
			PX_CORE_ERROR("Failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { vkSwapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		result = vkQueuePresentKHR(vkPresentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || s_FramebufferResized)
		{
			s_FramebufferResized = false;
			RecreateSwapChain();
		}
		else if (result != VK_SUCCESS) 
		{
			PX_CORE_ERROR("Failed to present swap chain image!");
		}

		s_CurrentFrame = (s_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanRenderer::WaitAndIdle()
	{
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

		vkDeviceWaitIdle(vkDevice);
	}

	void VulkanRenderer::RecreateSwapChain()
	{
		int width = 0, height = 0;

		auto vkWindowHandle = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwGetFramebufferSize(vkWindowHandle, &width, &height);
		while (width == 0 || height == 0) 
		{
			glfwGetFramebufferSize(vkWindowHandle, &width, &height);
			glfwWaitEvents();
		}

		WaitAndIdle();

		DestroyRecreatedSwapChain();

		m_SwapChain->CreateSwapChain();
		m_SwapChain->CreateImageViews();
		m_Framebuffer->CreateFramebuffers();
	}

	void VulkanRenderer::DestroyRecreatedSwapChain()
	{
		m_Framebuffer->DestroyFramebuffers();
		m_SwapChain->DestroyImageViews();
		m_SwapChain->DestroySwapChain();
	}
}