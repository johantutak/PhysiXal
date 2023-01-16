#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

	const int MAX_FRAMES_IN_FLIGHT = 2;

	class VulkanCommandBuffer
	{
	public:
		// Command buffer
		void CreateCommandBuffers();
		void DestroyCommandBuffers();

		static std::vector<VkCommandBuffer> GetVulkanCommandBuffers() { return s_CommandBuffers; }

		void RecordCommandBuffers(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	private:
		inline static std::vector<VkCommandBuffer> s_CommandBuffers;
	};
}