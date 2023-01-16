#include "px_pch.h"
#include "api/vulkan/vk_command_buffer.h"

#include "api/vulkan/vk_device.h"
#include "api/vulkan/vk_swap_chain.h"
#include "api/vulkan/vk_render_pass.h"
#include "api/vulkan/vk_framebuffer.h"
#include "api/vulkan/vk_pipeline.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

    void VulkanCommandBuffer::CreateCommandBuffers()
    {
        PX_CORE_INFO("Creating Vulkan command buffer");

        s_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

        VkCommandPool vkCommandPool = VulkanDevice::GetVulkanCommandPool();
        allocInfo.commandPool = vkCommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)s_CommandBuffers.size();

        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
        if (vkAllocateCommandBuffers(vkDevice, &allocInfo, s_CommandBuffers.data()) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to allocate command buffer!");
        }
    }

    void VulkanCommandBuffer::DestroyCommandBuffers()
    {
        PX_CORE_WARN("...Destroying Vulkan command buffer");

        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
        VkCommandPool vkCommandPool = VulkanDevice::GetVulkanCommandPool();
        vkDestroyCommandPool(vkDevice, vkCommandPool, nullptr);
    }

    void VulkanCommandBuffer::RecordCommandBuffers(VkCommandBuffer commandBuffer, uint32_t imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) 
        {
            PX_CORE_ERROR("Failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

        VkRenderPass vkRenderPass = VulkanRenderPass::GetVulkanRenderPass();
        renderPassInfo.renderPass = vkRenderPass;

        std::vector<VkFramebuffer> vkFramebuffer = VulkanFramebuffer::GetVulkanFramebuffer();
        renderPassInfo.framebuffer = vkFramebuffer[imageIndex];
        renderPassInfo.renderArea.offset = { 0, 0 };

        VkExtent2D vkSwapChainExtent2D = VulkanSwapChain::GetVulkanSwapChainExtent();
        renderPassInfo.renderArea.extent = vkSwapChainExtent2D;

        VkClearValue clearColor = { {{0.02f, 0.02f, 0.02f, 1.0f}} };
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
}

