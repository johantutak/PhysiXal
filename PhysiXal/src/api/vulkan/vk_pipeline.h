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

		// Shader
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
		static std::vector<char> ReadFile(const std::string& filename);

		// Render pass
		void CreateRenderPass();
		void DestroyRenderPass();
	private:
	VkPipeline m_GraphicsPipeline;

	inline static VkPipelineLayout s_PipelineLayout;

	VkRenderPass m_RenderPass;
	};
#endif
}