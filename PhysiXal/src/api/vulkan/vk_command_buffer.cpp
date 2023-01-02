#include "px_pch.h"
#include "api/vulkan/vk_command_buffer.h"

#include "api/vulkan/vk_device.h"
#include "api/vulkan/vk_framebuffer.h"
#include "api/vulkan/vk_pipeline.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

    void VulkanCommandBuffer::CreateCommandBuffer()
    {
        PX_CORE_INFO("Creating Vulkan command buffer");

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

        VkCommandPool vkCommandPool = VulkanDevice::GetVulkanCommandPool();
        allocInfo.commandPool = vkCommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
        if (vkAllocateCommandBuffers(vkDevice, &allocInfo, &m_CommandBuffer) != VK_SUCCESS) 
        {
            PX_CORE_ERROR("Failed to allocate command buffer!");
        }
    }

    void VulkanCommandBuffer::DestroyCommandBuffer()
    {
        PX_CORE_WARN("...Destroying Vulkan command buffer");


    }

    void VulkanCommandBuffer::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) 
        {
            PX_CORE_ERROR("Failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

        VkRenderPass vkRenderPass = VulkanPipeline::GetVulkanRenderPass();
        renderPassInfo.renderPass = vkRenderPass;

        std::vector<VkFramebuffer> vkFramebuffer = VulkanFramebuffer::GetVulkanFramebuffer();
        renderPassInfo.framebuffer = vkFramebuffer[imageIndex];
        renderPassInfo.renderArea.offset = { 0, 0 };

        VkExtent2D vkSwapChainExtent2D = VulkanDevice::GetVulkanSwapChainExtent();
        renderPassInfo.renderArea.extent = vkSwapChainExtent2D;

        VkClearValue clearColor = { {{0.1f, 0.1f, 0.1f, 1.0f}} };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkPipeline vkPipeline = VulkanPipeline::GetVulkanPipeline();
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

        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) 
        {
            PX_CORE_ERROR("Failed to record command buffer!");
        }
    }
#endif
}

