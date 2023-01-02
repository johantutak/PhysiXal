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
		void InitGraphicsPipeline();
		void ShutdownGraphicsPipeline();

		static VkPipeline GetVulkanPipeline() { return s_GraphicsPipeline; }

		// Shader
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
		static std::vector<char> ReadFile(const std::string& filename);

		// Render pass
		void CreateRenderPass();
		void DestroyRenderPass();

		static VkRenderPass GetVulkanRenderPass() { return s_RenderPass; }
	private:
		inline static VkPipeline s_GraphicsPipeline;

		inline static VkPipelineLayout s_PipelineLayout;

		inline static VkRenderPass s_RenderPass;
	};
#endif
}