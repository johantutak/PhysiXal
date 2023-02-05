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

#include "gui/gui.h"

namespace PhysiXal {

	// Vulkan pointers
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

	static Gui* m_Gui = nullptr;

	// Proxy functions
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

	void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat FindDepthFormat();

	VkSampleCountFlagBits GetMaxUsableSampleCount();
}