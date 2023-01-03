#pragma once

#ifdef PX_PLATFORM_WINDOWS
	#include <vulkan/vulkan.h>
#endif

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	class VulkanPipeline
	{
	public:
		// Graphics pipeline
		void CreateGraphicsPipeline();
		void DestroyGraphicsPipeline();

		static VkPipeline GetVulkanPipeline() { return s_GraphicsPipeline; }
	private:
		inline static VkPipeline s_GraphicsPipeline;
		inline static VkPipelineLayout s_PipelineLayout;
	};
#endif
}