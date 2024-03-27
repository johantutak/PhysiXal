#include "px_pch.h"
#include "api/vulkan/vk_uniform_buffer.h"

#include "api/vulkan/vk_utilities.h"
#include "api/vulkan/vk_initializers.h"

#include "core/application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "scene/camera.h"
#include "scene/component.h"

#include "asset/asset_manager.h"

namespace PhysiXal {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Descriptor set layout
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanUniformBuffer::CreateDescriptorSetLayout()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_INFO("Creating Vulkan descriptor set layouts");

        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 0;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        
        layoutInfo.pBindings = &uboLayoutBinding;

        if (vkCreateDescriptorSetLayout(VulkanDevice::GetVulkanDevice(), &layoutInfo, nullptr, &s_UniformDescriptorSetLayout) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create uniform descriptor set layout!");
        }
        else
        {
            PX_CORE_INFO("...For uniform,");
        }

        layoutInfo.pBindings = &samplerLayoutBinding;

        if (vkCreateDescriptorSetLayout(VulkanDevice::GetVulkanDevice(), &layoutInfo, nullptr, &s_TextureDescriptorSetLayout) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create texture descriptor set layout!");
        }
        else
        {
            PX_CORE_INFO("   for texture...");
        }
    }

    void VulkanUniformBuffer::DestroyDescriptorSetLayout()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_WARN("...Destroying Vulkan descriptor set layout");

        vkDestroyDescriptorSetLayout(VulkanDevice::GetVulkanDevice(), s_TextureDescriptorSetLayout, nullptr);

        vkDestroyDescriptorSetLayout(VulkanDevice::GetVulkanDevice(), s_UniformDescriptorSetLayout, nullptr);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Descriptor pool
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanUniformBuffer::CreateDescriptorPool()
    {
        // #### TODO #### Expand pool size and perhaps have it build in size or decrease depending on need. Rendering multiple objects will need it.
        // #### TODO #### Fix validation errors that are related to descriptors.


        PX_PROFILE_FUNCTION();

        PX_CORE_INFO("Creating Vulkan descriptor pool");

        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<U32>(c_MaxImageCount);
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = 3;

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.poolSizeCount = static_cast<U32>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<U32>(c_MaxImageCount) + 3;

        if (vkCreateDescriptorPool(VulkanDevice::GetVulkanDevice(), &poolInfo, nullptr, &s_DescriptorPool) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create descriptor pool!");
        }
    }

    void VulkanUniformBuffer::DestroyDescriptorPool()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_WARN("...Destroying Vulkan descriptor pool");

        vkDestroyDescriptorPool(VulkanDevice::GetVulkanDevice(), s_DescriptorPool, nullptr);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Descriptor Set
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanUniformBuffer::CreateUniformDescriptorSets()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_INFO("Creating Vulkan uniform descriptor sets");

        std::vector<VkDescriptorSetLayout> layouts(c_MaxImageCount, s_UniformDescriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = s_DescriptorPool;
        allocInfo.descriptorSetCount = static_cast<U32>(c_MaxImageCount);
        allocInfo.pSetLayouts = layouts.data();

        s_UniformDescriptorSets.resize(c_MaxImageCount);
        if (vkAllocateDescriptorSets(VulkanDevice::GetVulkanDevice(), &allocInfo, s_UniformDescriptorSets.data()) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to allocate descriptor sets!");
        }

        for (SIZE64 i = 0; i < c_MaxImageCount; i++)
        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = s_UniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            std::array<VkWriteDescriptorSet, 1> descriptorWrites{};
            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = s_UniformDescriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            vkUpdateDescriptorSets(VulkanDevice::GetVulkanDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }
    }

    void VulkanUniformBuffer::CreateTextureDescriptorSet()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_INFO("Creating Vulkan texture descriptor set");

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = s_DescriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &s_TextureDescriptorSetLayout;

        if (vkAllocateDescriptorSets(VulkanDevice::GetVulkanDevice(), &allocInfo, &s_TextureDescriptorSet) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to allocate descriptor set!");
        }

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = VulkanTexture::GetVulkanTexture().textureImageView;
        imageInfo.sampler = VulkanTexture::GetVulkanTexture().textureSampler;
   
        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = s_TextureDescriptorSet;
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(VulkanDevice::GetVulkanDevice(), 1, &descriptorWrite, 0, nullptr);
    }

    void VulkanUniformBuffer::DestroyUniformDescriptorSets()
    {
        PX_CORE_WARN("...Freeing Vulkan uniform descriptor sets");

        vkFreeDescriptorSets(VulkanDevice::GetVulkanDevice(), s_DescriptorPool, 1, s_UniformDescriptorSets.data());
    }

    void VulkanUniformBuffer::DestroyTextureDescriptorSet()
    {
        PX_CORE_WARN("...Freeing Vulkan texture descriptor set");

        vkFreeDescriptorSets(VulkanDevice::GetVulkanDevice(), s_DescriptorPool, 1, &s_TextureDescriptorSet);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Uniform buffer
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanUniformBuffer::CreateUniformBuffers()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_INFO("Creating Vulkan unifrom buffers");

        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        s_UniformBuffers.resize(c_MaxImageCount);
        s_UniformBuffersMemory.resize(c_MaxImageCount);
        s_UniformBuffersMapped.resize(c_MaxImageCount);

        for (SIZE64 i = 0; i < c_MaxImageCount; i++)
        {
            m_Buffer->CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                s_UniformBuffers[i], s_UniformBuffersMemory[i]);

            vkMapMemory(VulkanDevice::GetVulkanDevice(), s_UniformBuffersMemory[i], 0, bufferSize, 0, &s_UniformBuffersMapped[i]);
        }
    }

    void VulkanUniformBuffer::DestroyUnifromBuffers()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_WARN("...Destroying Vulkan unifrom buffers");

        for (SIZE64 i = 0; i < c_MaxImageCount; i++)
        {
            vkDestroyBuffer(VulkanDevice::GetVulkanDevice(), s_UniformBuffers[i], nullptr);
            vkFreeMemory(VulkanDevice::GetVulkanDevice(), s_UniformBuffersMemory[i], nullptr);
        }
    }

    void VulkanUniformBuffer::UpdateUniformBuffer(U32 currentImage)
    {
        //Transform::InitModelMatrix();

        // Set the position(x,y and z-axis), rotation (x,y and z-axis), and scale
        //Transform::SetPosition(glm::vec3(0.0f, -0.2f, 0.0f));
        //Transform::SetRotation(glm::vec3(-90.0f, 0.0f, -90.0f));
        //Transform::SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

        UniformBufferObject ubo{};
        ubo.model = Transform::GetModelMatrix();
        ubo.view = Camera::GetView();
        ubo.proj = Camera::GetProjection();
        ubo.proj[1][1] *= -1;

        memcpy(s_UniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
    }
}