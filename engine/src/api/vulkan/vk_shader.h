#pragma once


#include <vulkan/vulkan.h>

namespace PhysiXal {

	class VulkanShader
	{
	public:
		// Shader
		static VkShaderModule CreateShaderModule(const std::vector<char>& code);
		static std::vector<char> ReadFile(const std::string& filename);
	private:
	};
}