#include "px_pch.h"
#include "api/vulkan/vk_shader.h"

#include "api/vulkan/vk_utilities.h"
#include "api/vulkan/vk_initializers.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Shader
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    VkShaderModule VulkanShader::CreateShaderModule(const std::vector<char>& code)
    {

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const U32*>(code.data());

        VkShaderModule shaderModule;

        if (vkCreateShaderModule(VulkanDevice::GetVulkanDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create shader module!");
        }

        return shaderModule;
    }

    std::vector<char> VulkanShader::ReadFile(const std::string& filename)
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
    }
}

