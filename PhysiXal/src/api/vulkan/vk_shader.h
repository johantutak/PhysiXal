#pragma once

#ifdef PX_PLATFORM_WINDOWS
	#include <vulkan/vulkan.h>
#endif

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	class VulkanShader
	{
	public:
		// Shader
		static VkShaderModule CreateShaderModule(const std::vector<char>& code);
		static std::vector<char> ReadFile(const std::string& filename);
	private:
	};
#endif
}