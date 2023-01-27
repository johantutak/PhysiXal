#include "px_pch.h"
#include "api/vulkan/vk_command_buffer.h"

#include "api/vulkan/vk_device.h"
#include "api/vulkan/vk_swap_chain.h"
#include "api/vulkan/vk_render_pass.h"
#include "api/vulkan/vk_framebuffer.h"
#include "api/vulkan/vk_pipeline.h"
#include "api/vulkan/vk_buffer.h"
#include "api/vulkan/vk_uniform_buffer.h"
#include "api/vulkan/vk_renderer.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Command buffer
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanCommandBuffer::CreateCommandBuffers()
    {
        VkCommandPool vkCommandPool = VulkanDevice::GetVulkanCommandPool();
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_INFO("Creating Vulkan command buffer");

        s_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = vkCommandPool;
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
        VkCommandPool vkCommandPool = VulkanDevice::GetVulkanCommandPool();

        PX_CORE_WARN("...Destroying Vulkan command buffer");

        vkDestroyCommandPool(vkDevice, vkCommandPool, nullptr);
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

        VkClearValue clearColor = { {{0.02f, 0.02f, 0.02f, 1.0f}} };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

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

        vkCmdBindIndexBuffer(commandBuffer, vkIndexBuffer, 0, VK_INDEX_TYPE_UINT16);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout, 0, 1, &vkDescriptorSets[vkCurrentFrame], 0, nullptr);

        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) 
        {
            PX_CORE_ERROR("Failed to record command buffer!");
        }
    }
}

