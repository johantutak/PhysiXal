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
		void ShutdownGraphicsPipeline();

		// Shader
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
		static std::vector<char> ReadFile(const std::string& filename);
	private:
	inline static VkPipelineLayout s_PipelineLayout;
	};
#endif
}