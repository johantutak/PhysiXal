#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

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
}