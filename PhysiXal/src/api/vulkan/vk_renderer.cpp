#include "px_pch.h"
#include "api/vulkan/vk_renderer.h"

#include "api/vulkan/vk_context.h"
#include "api/vulkan/vk_device.h"
#include "api/vulkan/vk_swap_chain.h"
#include "api/vulkan/vk_pipeline.h"
#include "api/vulkan/vk_render_pass.h"
#include "api/vulkan/vk_framebuffer.h"
#include "api/vulkan/vk_command_buffer.h"

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
		m_Framebuffer->CreateFramebuffer();
		m_Device->CreateCommandPool();
		m_CommandBuffer->CreateCommandBuffers();
		CreateSyncObjects();
	}
	
	void VulkanRenderer::Shutdown()
	{
		PX_CORE_WARN("...Shutting down the renderer");

		DestroySyncObjects();
		m_CommandBuffer->DestroyCommandBuffers();
		m_Device->DestroyCommandPool();
		m_Framebuffer->DestroyFramebuffer();
		m_Pipeline->DestroyGraphicsPipeline();
		m_RenderPass->DestroyRenderPass();
		m_SwapChain->DestroyImageViews();
		m_SwapChain->DestroySwapChain();
		m_Device->DestroyDevice();
		m_Device->DestroySurface();
		m_Context->DestroyContext();
	}

	void VulkanRenderer::BeginFrame()
	{
		PX_CORE_INFO("Drawing frame!");

		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
		vkWaitForFences(vkDevice, 1, &s_InFlightFences[s_CurrentFrame], VK_TRUE, UINT64_MAX);
		vkResetFences(vkDevice, 1, &s_InFlightFences[s_CurrentFrame]);

		uint32_t imageIndex;

		VkSwapchainKHR vkSwapChain = VulkanSwapChain::GetVulkanSwapChain();
		vkAcquireNextImageKHR(vkDevice, vkSwapChain, UINT64_MAX, s_ImageAvailableSemaphores[s_CurrentFrame], VK_NULL_HANDLE, &imageIndex);

		std::vector<VkCommandBuffer> vkCommandBuffers = VulkanCommandBuffer::GetVulkanCommandBuffers();
		vkResetCommandBuffer(vkCommandBuffers[s_CurrentFrame], /*VkCommandBufferResetFlagBits*/ 0);
		m_CommandBuffer->RecordCommandBuffers(vkCommandBuffers[s_CurrentFrame], imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { s_ImageAvailableSemaphores[s_CurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &vkCommandBuffers[s_CurrentFrame];

		VkSemaphore signalSemaphores[] = { s_RenderFinishedSemaphores[s_CurrentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		VkQueue vkGraphicsQueue = VulkanDevice::GetVulkanGraphicsQueue();
		if (vkQueueSubmit(vkGraphicsQueue, 1, &submitInfo, s_InFlightFences[s_CurrentFrame]) != VK_SUCCESS)
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
		
		VkQueue vkPresentQueue = VulkanDevice::GetVulkanPresentQueue();
		vkQueuePresentKHR(vkPresentQueue, &presentInfo);

		s_CurrentFrame = (s_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanRenderer::WaitAndIdle()
	{
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
		vkDeviceWaitIdle(vkDevice);
	}

	void VulkanRenderer::CreateSyncObjects()
	{
		PX_CORE_INFO("Creating sync objects");

		s_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		s_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		s_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
		{
			VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
			if (vkCreateSemaphore(vkDevice, &semaphoreInfo, nullptr, &s_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(vkDevice, &semaphoreInfo, nullptr, &s_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(vkDevice, &fenceInfo, nullptr, &s_InFlightFences[i]) != VK_SUCCESS)
			{
				PX_CORE_ERROR("Failed to create synchronization objects for a frame!");
			}
		}
	}

	void VulkanRenderer::DestroySyncObjects()
	{
		PX_CORE_WARN("...Destroying sync objects");

		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
		{
			vkDestroySemaphore(vkDevice, s_ImageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(vkDevice, s_RenderFinishedSemaphores[i], nullptr);
			vkDestroyFence(vkDevice, s_InFlightFences[i], nullptr);
		}
	}
}