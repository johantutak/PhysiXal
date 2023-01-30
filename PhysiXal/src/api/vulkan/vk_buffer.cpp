#include "px_pch.h"
#include "api/vulkan/vk_buffer.h"

#include "api/vulkan/vk_utilities.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Buffer creation (staging buffer)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanBuffer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
    {
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(vkDevice, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(vkDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to allocate buffer memory!");
        }

        vkBindBufferMemory(vkDevice, buffer, bufferMemory, 0);
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
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
        std::vector<Vertex> vertices = VulkanModel::GetVulkanVertices();
        std::vector<uint32_t> indices = VulkanModel::GetVulkanIndices();

        PX_CORE_INFO("Creating Vulkan vertex buffer");

        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(vkDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(vkDevice, stagingBufferMemory);

        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, s_VertexBuffer, s_VertexBufferMemory);

        CopyBuffer(stagingBuffer, s_VertexBuffer, bufferSize);
    }

    void VulkanBuffer::DestroyVertexBuffer()
    {
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_WARN("...Destroying Vulkan vertex buffer");

        vkDestroyBuffer(vkDevice, s_VertexBuffer, nullptr);
        vkFreeMemory(vkDevice, s_VertexBufferMemory, nullptr);
    }

    uint32_t VulkanBuffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDevice vkPhysicalDevice = VulkanDevice::GetVulkanPhysicalDevice();

        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
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
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
        std::vector<uint32_t> indices = VulkanModel::GetVulkanIndices();

        PX_CORE_INFO("Creating Vulkan index buffer");

        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(vkDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (size_t)bufferSize);
        vkUnmapMemory(vkDevice, stagingBufferMemory);

        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, s_IndexBuffer, s_IndexBufferMemory);

        CopyBuffer(stagingBuffer, s_IndexBuffer, bufferSize);

        vkDestroyBuffer(vkDevice, stagingBuffer, nullptr);
        vkFreeMemory(vkDevice, stagingBufferMemory, nullptr);
    }

    void VulkanBuffer::DestroyIndexBuffer()
    {
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_WARN("...Destroying Vulkan index buffer");

        vkDestroyBuffer(vkDevice, s_IndexBuffer, nullptr);
        vkFreeMemory(vkDevice, s_IndexBufferMemory, nullptr);
    }
}