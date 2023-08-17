#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

	class VulkanPipeline
	{
	public:
		// Graphics pipeline
		void CreateGraphicsPipeline(const std::string& vertexShader, const std::string& fragmentShader);
		void DestroyGraphicsPipeline();

		static VkPipeline GetVulkanPipeline() { return s_GraphicsPipeline; }
		static VkPipelineLayout GetVulkanPipelineLayout() { return s_PipelineLayout; }
	private:
		inline static VkPipeline s_GraphicsPipeline;
		inline static VkPipelineLayout s_PipelineLayout;
	};
}