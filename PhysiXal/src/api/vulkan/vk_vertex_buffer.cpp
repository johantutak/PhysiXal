#include "px_pch.h"
#include "api/vulkan/vk_vertex_buffer.h"

#include "api/vulkan/vk_device.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

    void VulkanVertexBuffer::CreateVertexBuffer()
    {
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_INFO("Creating Vulkan vertex buffer");

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = sizeof(vertices[0]) * vertices.size();
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &s_VertexBuffer) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create vertex buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(vkDevice, s_VertexBuffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (vkAllocateMemory(vkDevice, &allocInfo, nullptr, &s_VertexBufferMemory) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to allocate vertex buffer memory!");
        }

        vkBindBufferMemory(vkDevice, s_VertexBuffer, s_VertexBufferMemory, 0);

        void* data;
        vkMapMemory(vkDevice, s_VertexBufferMemory, 0, bufferInfo.size, 0, &data);
        memcpy(data, vertices.data(), (size_t)bufferInfo.size);
        vkUnmapMemory(vkDevice, s_VertexBufferMemory);
    }

    void VulkanVertexBuffer::DestroyVertexBuffer()
    {
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_WARN("...Destroying Vulkan vertex buffer");

        vkDestroyBuffer(vkDevice, s_VertexBuffer, nullptr);
        vkFreeMemory(vkDevice, s_VertexBufferMemory, nullptr);
    }

    uint32_t VulkanVertexBuffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
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
}