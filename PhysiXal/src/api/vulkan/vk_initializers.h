#pragma once

#include "api/vulkan/vk_renderer.h"
#include "api/vulkan/vk_context.h"
#include "api/vulkan/vk_device.h"
#include "api/vulkan/vk_swap_chain.h"
#include "api/vulkan/vk_shader.h"
#include "api/vulkan/vk_pipeline.h"
#include "api/vulkan/vk_render_pass.h"
#include "api/vulkan/vk_framebuffer.h"
#include "api/vulkan/vk_command_buffer.h"
#include "api/vulkan/vk_sync_objects.h"
#include "api/vulkan/vk_buffer.h"
#include "api/vulkan/vk_uniform_buffer.h"
#include "api/vulkan/vk_texture.h"
#include "api/vulkan/vk_depth_buffer.h"
#include "api/vulkan/vk_model.h"

namespace PhysiXal {

	// Vulkan pointers (initializers)
	static VulkanRenderer* m_VulkanRenderer = nullptr;
	static VulkanContext* m_Context = nullptr;
	static VulkanDevice* m_Device = nullptr;
	static VulkanSwapChain* m_SwapChain = nullptr;
	static VulkanRenderPass* m_RenderPass = nullptr;
	static VulkanPipeline* m_Pipeline = nullptr;
	static VulkanFramebuffer* m_Framebuffer = nullptr;
	static VulkanCommandBuffer* m_CommandBuffer = nullptr;
	static VulkanSyncObjects* m_SyncObjects = nullptr;
	static VulkanBuffer* m_Buffer = nullptr;
	static VulkanUniformBuffer* m_UniformBuffer = nullptr;
	static VulkanTexture* m_Texture = nullptr;
	static VulkanDepthBuffer* m_DepthBuffer = nullptr;
	static VulkanModel* m_Model = nullptr;
}