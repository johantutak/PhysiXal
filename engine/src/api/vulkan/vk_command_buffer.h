#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

	const int c_MaxImageCount = 2;

	class VulkanCommandBuffer
	{
	public:
		// Command buffer
		void CreateCommandBuffers();
		void DestroyCommandBuffers();

		static std::vector<VkCommandBuffer> GetVulkanCommandBuffers() { return s_CommandBuffers; }

		void RecordCommandBuffers(VkCommandBuffer commandBuffer, U32 imageIndex);

		void UpdateCommandBuffers(VkCommandBuffer commandBuffer, VkBuffer vertexBuffer, VkBuffer indexBuffer, uint32_t indexCount);

		// Command pool
		void CreateCommandPool();
		void DestroyCommandPool();

		static VkCommandPool GetVulkanCommandPool() { return s_CommandPool; }
	private:
		inline static std::vector<VkCommandBuffer> s_CommandBuffers;

		inline static VkCommandPool s_CommandPool;
	};
}