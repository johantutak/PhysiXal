#include "px_pch.h"
#include "api/vulkan/vk_renderer.h"

#include "api/vulkan/vk_utilities.h"
#include "api/vulkan/vk_initializers.h"

#include "api/imgui/imgui_initializers.h"

#include "core/application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Vulkan renderer
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void VulkanRenderer::Init()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_INFO("Initializing the renderer");

		m_Context->CreateContext();
		m_Context->SetupDebugMessenger();
		m_Device->CreateSurface();
		m_Device->PickPhysicalDevice();
		m_Device->CreateLogicalDevice();
		m_SwapChain->CreateSwapChain();
		m_SwapChain->CreateImageViews();	
		m_RenderPass->CreateRenderPass();
		m_UniformBuffer->CreateDescriptorSetLayout();
		m_Pipeline->CreateGraphicsPipeline();
		m_CommandBuffer->CreateCommandPool();
		m_Device->CreateColorResources();
		m_DepthBuffer->CreateDepthResources();
		m_Framebuffer->CreateFramebuffers();
		m_Texture->CreateTextureImage();
		m_Texture->CreateTextureImageView();
		m_Texture->CreateTextureSampler();
		m_Model->LoadModel();
		m_Buffer->CreateVertexBuffer();
		m_Buffer->CreateIndexBuffer();
		m_UniformBuffer->CreateUniformBuffers();
		m_UniformBuffer->CreateDescriptorPool();
		m_UniformBuffer->CreateDescriptorSets();
		m_CommandBuffer->CreateCommandBuffers();
		m_SyncObjects->CreateSyncObjects();
	}
	
	void VulkanRenderer::Shutdown()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_WARN("...Shutting down the renderer");

		m_SwapChain->DestroySwapChain();
		m_Pipeline->DestroyGraphicsPipeline();
		m_RenderPass->DestroyRenderPass();
		m_UniformBuffer->DestroyUnifromBuffers();
		m_UniformBuffer->DestroyDescriptorPool();
		m_Texture->DestroyTextureSampler();
		m_Texture->DestroyTextureImageView();
		m_Texture->DestroyTextureImage();
		m_UniformBuffer->DestroyDescriptorSetLayout();
		m_Buffer->DestroyIndexBuffer();
		m_Buffer->DestroyVertexBuffer();
		m_SyncObjects->DestroySyncObjects();
		m_CommandBuffer->DestroyCommandBuffers();
		m_CommandBuffer->DestroyCommandPool();
		m_Device->DestroyDevice();
		m_Context->DestroyDebugMessenger();
		m_Device->DestroySurface();
		m_Context->DestroyContext();
	}

	void VulkanRenderer::DrawFrame()
	{
		PX_PROFILE_SCOPE("Renderer Draw");

		PX_CORE_INFO("Drawing frame!");
	
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
		std::vector<VkFence> vkInFlightFences = VulkanSyncObjects::GetVulkanFences();
		VkSwapchainKHR vkSwapChain = VulkanSwapChain::GetVulkanSwapChain();
		std::vector<VkSemaphore> vkImageSemaphores = VulkanSyncObjects::GetVulkanImageSemaphores();
		std::vector<VkCommandBuffer> vkCommandBuffers = VulkanCommandBuffer::GetVulkanCommandBuffers();
		std::vector<VkCommandBuffer> vkGuiCommandBuffers = GuiVulkan::GetGuiCommandBuffers();
		std::vector<VkSemaphore> vkRenderSemaphores = VulkanSyncObjects::GetVulkanRenderSemaphores();
		VkQueue vkGraphicsQueue = VulkanDevice::GetVulkanGraphicsQueue();
		VkQueue vkPresentQueue = VulkanDevice::GetVulkanPresentQueue();

		vkWaitForFences(vkDevice, 1, &vkInFlightFences[s_CurrentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;

		VkResult result = vkAcquireNextImageKHR(vkDevice, vkSwapChain, UINT64_MAX, vkImageSemaphores[s_CurrentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) 
		{
			m_SwapChain->RecreateSwapChain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) 
		{
			PX_CORE_ERROR("Failed to acquire swap chain image!");
		}
		
		//vkResetCommandBuffer(vkCommandBuffers[s_CurrentFrame], /*VkCommandBufferResetFlagBits*/ 0);
		
		m_CommandBuffer->RecordCommandBuffers(vkCommandBuffers[s_CurrentFrame], imageIndex);
		m_Gui->GuiDraw(vkGuiCommandBuffers[s_CurrentFrame], imageIndex);

		m_UniformBuffer->UpdateUniformBuffer(s_CurrentFrame);

		// Only reset the fence if we are submitting work
		vkResetFences(vkDevice, 1, &vkInFlightFences[s_CurrentFrame]);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		std::array<VkCommandBuffer, 2> submitCommandBuffers = { vkCommandBuffers[s_CurrentFrame], vkGuiCommandBuffers[s_CurrentFrame] };

		VkSemaphore waitSemaphores[] = { vkImageSemaphores[s_CurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = static_cast<uint32_t>(submitCommandBuffers.size());
		submitInfo.pCommandBuffers = submitCommandBuffers.data();

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
			m_SwapChain->RecreateSwapChain();
		}
		else if (result != VK_SUCCESS) 
		{
			PX_CORE_ERROR("Failed to present swap chain image!");
		}

		s_CurrentFrame = (s_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanRenderer::WaitAndIdle()
	{
		PX_PROFILE_FUNCTION();

		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

		vkDeviceWaitIdle(vkDevice);
	}
}