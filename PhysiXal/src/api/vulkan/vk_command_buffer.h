#pragma once

#ifdef PX_PLATFORM_WINDOWS
	#include <vulkan/vulkan.h>
#endif

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	class VulkanCommandBuffer
	{
	public:
		// Command buffer
		void CreateCommandBuffer();
		void DestroyCommandBuffer();

		void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	private:
		VkCommandBuffer m_CommandBuffer;
	};
#endif
}