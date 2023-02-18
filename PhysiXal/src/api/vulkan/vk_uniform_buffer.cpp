#include "px_pch.h"
#include "api/vulkan/vk_uniform_buffer.h"

#include "api/vulkan/vk_utilities.h"
#include "api/vulkan/vk_initializers.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

namespace PhysiXal {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Descriptor set layout
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanUniformBuffer::CreateDescriptorSetLayout()
    {
        PX_PROFILE_FUNCTION();

        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_INFO("Creating Vulkan descriptor set layout");

        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(vkDevice, &layoutInfo, nullptr, &s_DescriptorSetLayout) != VK_SUCCESS) 
        {
            PX_CORE_ERROR("Failed to create descriptor set layout!");
        }
    }

    void VulkanUniformBuffer::DestroyDescriptorSetLayout()
    {
        PX_PROFILE_FUNCTION();

        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_WARN("...Destroying Vulkan descriptor set layout");
        
        vkDestroyDescriptorSetLayout(vkDevice, s_DescriptorSetLayout, nullptr);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Descriptor pool
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanUniformBuffer::CreateDescriptorPool()
    {
        PX_PROFILE_FUNCTION();

        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_INFO("Creating Vulkan descriptor pool");

        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        if (vkCreateDescriptorPool(vkDevice, &poolInfo, nullptr, &s_DescriptorPool) != VK_SUCCESS) 
        {
            PX_CORE_ERROR("Failed to create descriptor pool!");
        }
    }
    
    void VulkanUniformBuffer::DestroyDescriptorPool()
    {
        PX_PROFILE_FUNCTION();

        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_WARN("...Destroying Vulkan descriptor pool");

        vkDestroyDescriptorPool(vkDevice, s_DescriptorPool, nullptr);
    }

    void VulkanUniformBuffer::CreateDescriptorSets()
    {
        PX_PROFILE_FUNCTION();

        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
        VkImageView vkTextureImageView = VulkanTexture::GetVulkanTextureImageView();
        VkSampler vkTextureSampler = VulkanTexture::GetVulkanTextureSampler();

        PX_CORE_INFO("Creating Vulkan descriptor sets");
        
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, s_DescriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = s_DescriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts.data();

        s_DescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
        if (vkAllocateDescriptorSets(vkDevice, &allocInfo, s_DescriptorSets.data()) != VK_SUCCESS) 
        {
            PX_CORE_ERROR("Failed to allocate descriptor sets!");
        }

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = s_UniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = vkTextureImageView;
            imageInfo.sampler = vkTextureSampler;

            std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = s_DescriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = s_DescriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &imageInfo;

            vkUpdateDescriptorSets(vkDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Uniform buffer
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanUniformBuffer::CreateUniformBuffers()
    {
        PX_PROFILE_FUNCTION();

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
        PX_PROFILE_FUNCTION();

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