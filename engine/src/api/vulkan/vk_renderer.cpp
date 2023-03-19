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

		// Create the context (instance)
		m_Context->CreateContext();

		// Create the debug messenger
		m_Context->SetupDebugMessenger();

		// Create presentation surface
		m_Context->CreateSurface();

		// Enumerate physical devices, get their properties, find queue family/families supporting graphics and present
		m_Device->PickPhysicalDevice();

		// Create logical device and queues
		m_Device->CreateLogicalDevice();

		// Create swap chain
		m_SwapChain->CreateSwapChain();

		// Create image views
		m_SwapChain->CreateImageViews();	

		// Create render pass
		m_RenderPass->CreateRenderPass();

		// Create descriptor set layout
		m_UniformBuffer->CreateDescriptorSetLayout();

		// Create pipeline
		m_Pipeline->CreateGraphicsPipeline();

		// Create command pool
		m_CommandBuffer->CreateCommandPool();

		// Create color resources for multisampling (MSAA)
		m_Device->CreateColorResources();

		// Create depth resources for the depth buffer 
		m_DepthBuffer->CreateDepthResources();

		// Create Framebuffers
		m_Framebuffer->CreateFramebuffers();

		// Create texture image
		m_Texture->CreateTextureImage();

		// Create texture image views
		m_Texture->CreateTextureImageView();

		// Create texture sampler for LOD
		m_Texture->CreateTextureSampler();

		// Load model (.obj) to renderer
		m_Model->LoadModel();

		// Create vertex buffer
		m_Buffer->CreateVertexBuffer();

		// Create index buffer
		m_Buffer->CreateIndexBuffer();

		// Create uniform buffer
		m_UniformBuffer->CreateUniformBuffers();

		// Create descriptor pool
		m_UniformBuffer->CreateDescriptorPool();

		// Create descriptor sets
		m_UniformBuffer->CreateDescriptorSets();

		// Create command buffers
		m_CommandBuffer->CreateCommandBuffers();

		// Create synchronization objects as in semaphores for image acquisition and rendering completion and render fence
		m_SyncObjects->CreateSyncObjects();
	}
	
	void VulkanRenderer::Shutdown()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_WARN("...Shutting down the renderer");

		// Destruction of the swap chain and collection of other functions connected to it
		m_SwapChain->DestroySwapChain();

		// Destruction of the pipeline
		m_Pipeline->DestroyGraphicsPipeline();

		// Destruction of the render pass 
		m_RenderPass->DestroyRenderPass();

		// Destruction of the uniform buffers
		m_UniformBuffer->DestroyUnifromBuffers();

		// Destruction of the descriptor pool
		m_UniformBuffer->DestroyDescriptorPool();

		// Destruction of the texture sampler for LOD
		m_Texture->DestroyTextureSampler();

		// Destruction the texture image view
		m_Texture->DestroyTextureImageView();

		// Destruction of the texture image
		m_Texture->DestroyTextureImage();

		// Destruction of the descriptor set layout
		m_UniformBuffer->DestroyDescriptorSetLayout();

		// Destruction of the index buffer
		m_Buffer->DestroyIndexBuffer();

		// Destruction of the vertex buffer
		m_Buffer->DestroyVertexBuffer();

		// Destruction of the synchronization objects which includes the semaphores and render fence
		m_SyncObjects->DestroySyncObjects();

		// Destruction of the command buffers
		m_CommandBuffer->DestroyCommandBuffers();

		// Destruction of the command pool
		m_CommandBuffer->DestroyCommandPool();

		// Destruction of the logical device
		m_Device->DestroyDevice();

		// Destruction of the debug messenger
		m_Context->DestroyDebugMessenger();

		// Destruction of the surface
		m_Context->DestroySurface();

		// Destruction of the context (instance)
		m_Context->DestroyContext();
	}

	void VulkanRenderer::BeginFrame()
	{
		PX_PROFILE_SCOPE("Renderer Begin Frame");

		PX_CORE_INFO("Drawing frame!");

		vkWaitForFences(VulkanDevice::GetVulkanDevice(), 1, &VulkanSyncObjects::GetVulkanFences()[s_CurrentFrame], VK_TRUE, UINT64_MAX);

		VkResult result = vkAcquireNextImageKHR(VulkanDevice::GetVulkanDevice(), VulkanSwapChain::GetVulkanSwapChain(), UINT64_MAX,
			VulkanSyncObjects::GetVulkanImageSemaphores()[s_CurrentFrame], VK_NULL_HANDLE, &s_ImageIndex);

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
		
		m_CommandBuffer->RecordCommandBuffers(VulkanCommandBuffer::GetVulkanCommandBuffers()[s_CurrentFrame], s_ImageIndex);
	}

	void VulkanRenderer::EndFrame()
	{
		PX_PROFILE_SCOPE("Renderer End Frame");

		m_UniformBuffer->UpdateUniformBuffer(s_CurrentFrame);

		// Only reset the fence if we are submitting work
		vkResetFences(VulkanDevice::GetVulkanDevice(), 1, &VulkanSyncObjects::GetVulkanFences()[s_CurrentFrame]);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		std::array<VkCommandBuffer, 2> submitCommandBuffers = { VulkanCommandBuffer::GetVulkanCommandBuffers()[s_CurrentFrame], GuiVulkan::GetGuiCommandBuffers()[s_CurrentFrame] };

		VkSemaphore waitSemaphores[] = { VulkanSyncObjects::GetVulkanImageSemaphores()[s_CurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = static_cast<U32>(submitCommandBuffers.size());
		submitInfo.pCommandBuffers = submitCommandBuffers.data();

		VkSemaphore signalSemaphores[] = { VulkanSyncObjects::GetVulkanRenderSemaphores()[s_CurrentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(VulkanDevice::GetVulkanGraphicsQueue(), 1, &submitInfo, VulkanSyncObjects::GetVulkanFences()[s_CurrentFrame]) != VK_SUCCESS)
		{
			PX_CORE_ERROR("Failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { VulkanSwapChain::GetVulkanSwapChain() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &s_ImageIndex;
		VkResult result = vkQueuePresentKHR(VulkanDevice::GetVulkanPresentQueue(), &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || s_FramebufferResized)
		{
			s_FramebufferResized = false;
			m_SwapChain->RecreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			PX_CORE_ERROR("Failed to present swap chain image!");
		}

		s_CurrentFrame = (s_CurrentFrame + 1) % c_MaxImageCount;
	}

	void VulkanRenderer::WaitAndIdle()
	{
		PX_PROFILE_FUNCTION();

		vkDeviceWaitIdle(VulkanDevice::GetVulkanDevice());
	}
}