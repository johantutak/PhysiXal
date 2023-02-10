#include "px_pch.h"
#include "api/vulkan/vk_command_buffer.h"

#include "api/vulkan/vk_utilities.h"
#include "api/vulkan/vk_initializers.h"
#include "api/vulkan/vk_renderer.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Command buffer
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanCommandBuffer::CreateCommandBuffers() 
    {
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
        std::vector<VkImageView> vkSwapChainImages = VulkanSwapChain::GetVulkanImageViews();

        PX_CORE_INFO("Allocating Vulkan command buffers");

        s_CommandBuffers.resize(vkSwapChainImages.size());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = s_CommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)s_CommandBuffers.size();
        
        if (vkAllocateCommandBuffers(vkDevice, &allocInfo, s_CommandBuffers.data()) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to allocate command buffer!");
        }
    }


    void VulkanCommandBuffer::DestroyCommandBuffers()
    {
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_WARN("...Freeing up Vulkan command buffers");

        vkFreeCommandBuffers(vkDevice, s_CommandPool, static_cast<uint32_t>(s_CommandBuffers.size()), s_CommandBuffers.data());
    }

    void VulkanCommandBuffer::RecordCommandBuffers(VkCommandBuffer commandBuffer, uint32_t imageIndex)
    {
        VkRenderPass vkRenderPass = VulkanRenderPass::GetVulkanRenderPass();
        std::vector<VkFramebuffer> vkFramebuffer = VulkanFramebuffer::GetVulkanFramebuffers();
        VkExtent2D vkSwapChainExtent2D = VulkanSwapChain::GetVulkanSwapChainExtent();
        VkPipeline vkPipeline = VulkanPipeline::GetVulkanPipeline();
        VkPipelineLayout vkPipelineLayout = VulkanPipeline::GetVulkanPipelineLayout();
        VkBuffer vkVertexBuffer = VulkanBuffer::GetVulkanVertexBuffer();
        VkBuffer vkIndexBuffer = VulkanBuffer::GetVulkanIndexBuffer();
        std::vector<VkDescriptorSet> vkDescriptorSets = VulkanUniformBuffer::GetVulkanDescriptorSets();
        uint32_t vkCurrentFrame = VulkanRenderer::GetVulkanCurrentFrame();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) 
        {
            PX_CORE_ERROR("Failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = vkRenderPass;
        renderPassInfo.framebuffer = vkFramebuffer[imageIndex];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = vkSwapChainExtent2D;

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { {0.02f, 0.02f, 0.02f, 1.0f} };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)vkSwapChainExtent2D.width;
        viewport.height = (float)vkSwapChainExtent2D.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = vkSwapChainExtent2D;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        VkBuffer vertexBuffers[] = { vkVertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffer, vkIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout, 0, 1, &vkDescriptorSets[vkCurrentFrame], 0, nullptr);

        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_Model->GetVulkanIndices().size()), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) 
        {
            PX_CORE_ERROR("Failed to record command buffer!");
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  Command pool
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanCommandBuffer::CreateCommandPool()
    {
        VkPhysicalDevice vkPhysicalDevice = VulkanDevice::GetVulkanPhysicalDevice();
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_INFO("Creating Vulkan command pool");

        QueueFamilyIndices queueFamilyIndices = m_Device->FindQueueFamilies(vkPhysicalDevice);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.m_GraphicsFamily.value();

        if (vkCreateCommandPool(vkDevice, &poolInfo, nullptr, &s_CommandPool) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create command pool!");
        }
    }

    void VulkanCommandBuffer::DestroyCommandPool()
    {
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_WARN("...Destroying Vulkan command pool");

        // Destroys command buffers that are in the pool
        vkDestroyCommandPool(vkDevice, s_CommandPool, nullptr);
    }
}

