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

		static VkCommandBuffer GetVulkanCommandBuffer() { return s_CommandBuffer; }

		void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	private:
		inline static VkCommandBuffer s_CommandBuffer;
	};
#endif
}