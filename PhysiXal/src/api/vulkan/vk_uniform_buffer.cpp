#include "px_pch.h"
#include "api/vulkan/vk_uniform_buffer.h"

#include "api/vulkan/vk_device.h"
#include "api/vulkan/vk_buffer.h"
#include "api/vulkan/vk_command_buffer.h"
#include "api/vulkan/vk_swap_chain.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

namespace PhysiXal {

    static VulkanBuffer* m_Buffer = nullptr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Descriptor pool
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanUniformBuffer::CreateDescriptorSetLayout()
    {
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_INFO("Creating Vulkan descriptor pool");

        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;

        if (vkCreateDescriptorSetLayout(vkDevice, &layoutInfo, nullptr, &s_DescriptorSetLayout) != VK_SUCCESS) 
        {
            PX_CORE_ERROR("Failed to create descriptor set layout!");
        }
    }

    void VulkanUniformBuffer::DestroyDescriptorSetLayout()
    {
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_WARN("...Destroying Vulkan descriptor pool");
        
        vkDestroyDescriptorSetLayout(vkDevice, s_DescriptorSetLayout, nullptr);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Vertex buffer
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanUniformBuffer::CreateUniformBuffers()
    {
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_INFO("Creating Vulkan unifrom buffers");

        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        s_UniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        s_UniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
        s_UniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            m_Buffer->CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, s_UniformBuffers[i], s_UniformBuffersMemory[i]);

            vkMapMemory(vkDevice, s_UniformBuffersMemory[i], 0, bufferSize, 0, &s_UniformBuffersMapped[i]);
        }
    }

    void VulkanUniformBuffer::DestroyUnifromBuffers()
    {
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_WARN("...Destroying Vulkan unifrom buffers");

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(vkDevice, s_UniformBuffers[i], nullptr);
            vkFreeMemory(vkDevice, s_UniformBuffersMemory[i], nullptr);
        }
    }

    void VulkanUniformBuffer::UpdateUniformBuffer(uint32_t currentImage)
    {
        VkExtent2D vkSwapChainExtent2D = VulkanSwapChain::GetVulkanSwapChainExtent();

        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), vkSwapChainExtent2D.width / (float)vkSwapChainExtent2D.height, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;

        memcpy(s_UniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
    }
}