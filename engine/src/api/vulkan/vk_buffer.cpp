#include "px_pch.h"
#include "api/vulkan/vk_buffer.h"

#include "api/vulkan/vk_utilities.h"
#include "api/vulkan/vk_initializers.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Buffer creation (staging buffer)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanBuffer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(VulkanDevice::GetVulkanDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(VulkanDevice::GetVulkanDevice(), buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(VulkanDevice::GetVulkanDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to allocate buffer memory!");
        }

        vkBindBufferMemory(VulkanDevice::GetVulkanDevice(), buffer, bufferMemory, 0);
    }

    void VulkanBuffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
    {
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        EndSingleTimeCommands(commandBuffer);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Vertex buffer
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanBuffer::CreateVertexBuffer()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_INFO("Creating Vulkan vertex buffer");

        VkDeviceSize bufferSize = sizeof(m_Model->GetVulkanVertices()[0]) * m_Model->GetVulkanVertices().size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(VulkanDevice::GetVulkanDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, m_Model->GetVulkanVertices().data(), (size_t)bufferSize);
        vkUnmapMemory(VulkanDevice::GetVulkanDevice(), stagingBufferMemory);

        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, s_VertexBuffer, s_VertexBufferMemory);

        CopyBuffer(stagingBuffer, s_VertexBuffer, bufferSize);
    }

    void VulkanBuffer::DestroyVertexBuffer()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_WARN("...Destroying Vulkan vertex buffer");

        vkDestroyBuffer(VulkanDevice::GetVulkanDevice(), s_VertexBuffer, nullptr);
        vkFreeMemory(VulkanDevice::GetVulkanDevice(), s_VertexBufferMemory, nullptr);
    }

    U32 VulkanBuffer::FindMemoryType(U32 typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(VulkanDevice::GetVulkanPhysicalDevice(), &memProperties);

        for (U32 i = 0; i < memProperties.memoryTypeCount; i++) 
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        PX_CORE_ERROR("Failed to find suitable memory type!");
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Index buffer
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanBuffer::CreateIndexBuffer()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_INFO("Creating Vulkan index buffer");

        VkDeviceSize bufferSize = sizeof(m_Model->GetVulkanIndices()[0]) * m_Model->GetVulkanIndices().size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(VulkanDevice::GetVulkanDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, m_Model->GetVulkanIndices().data(), (size_t)bufferSize);
        vkUnmapMemory(VulkanDevice::GetVulkanDevice(), stagingBufferMemory);

        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, s_IndexBuffer, s_IndexBufferMemory);

        CopyBuffer(stagingBuffer, s_IndexBuffer, bufferSize);

        vkDestroyBuffer(VulkanDevice::GetVulkanDevice(), stagingBuffer, nullptr);
        vkFreeMemory(VulkanDevice::GetVulkanDevice(), stagingBufferMemory, nullptr);
    }

    void VulkanBuffer::DestroyIndexBuffer()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_WARN("...Destroying Vulkan index buffer");

        vkDestroyBuffer(VulkanDevice::GetVulkanDevice(), s_IndexBuffer, nullptr);
        vkFreeMemory(VulkanDevice::GetVulkanDevice(), s_IndexBufferMemory, nullptr);
    }
}