#include "px_pch.h"
#include "api/vulkan/vk_pipeline.h"

#include "api/vulkan/vk_device.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

/*VkShaderModule CreateShaderModule(const std::vector<char>& code) {
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;

	VkDevice vkDevice = PhysiXal::VulkanDevice::GetVulkanDevice();

	if (vkCreateShaderModule(vkDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		PX_CORE_ERROR("Failed to create shader module!");
	}

	return shaderModule;
}*/

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	/*void VulkanPipeline::CreateGraphicsPipeline()
	{
		PX_CORE_INFO("Creating graphics pipeline");

		auto vertShaderCode = ReadFile("../../../Example/assets/shaders/base_vert.spv");
		auto fragShaderCode = ReadFile("../../../Example/assets/shaders/base_frag.spv");

		VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
		VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
		
		vkDestroyShaderModule(vkDevice, fragShaderModule, nullptr);
		vkDestroyShaderModule(vkDevice, vertShaderModule, nullptr);
	}

	void VulkanPipeline::ShutdownGraphicsPipeline()
	{
		PX_CORE_WARN("...Shutting down the graphics pipeline");
	}

	std::vector<char> VulkanPipeline::ReadFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) 
		{
			PX_CORE_ERROR("Failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}*/
#endif
}

