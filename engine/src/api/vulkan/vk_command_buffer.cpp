#include "px_pch.h"
#include "api/vulkan/vk_command_buffer.h"

#include "api/vulkan/vk_utilities.h"
#include "api/vulkan/vk_initializers.h"
#include "api/vulkan/vk_renderer.h"

#include "asset/asset_manager.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Command buffer
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanCommandBuffer::CreateCommandBuffers() 
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_INFO("Allocating Vulkan command buffers");

        s_CommandBuffers.resize(VulkanSwapChain::GetVulkanImageViews().size());

        // Allocation of  multiple command buffers are possible
        // 
        // The class takes in the command pool that was created
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = s_CommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (U32)s_CommandBuffers.size();
        
        if (vkAllocateCommandBuffers(VulkanDevice::GetVulkanDevice(), &allocInfo, s_CommandBuffers.data()) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to allocate command buffer!");
        }
    }


    void VulkanCommandBuffer::DestroyCommandBuffers()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_WARN("...Freeing up Vulkan command buffers");

        // Frees command buffers that are in the pool
        vkFreeCommandBuffers(VulkanDevice::GetVulkanDevice(), s_CommandPool, static_cast<U32>(s_CommandBuffers.size()), s_CommandBuffers.data());
    }

    void VulkanCommandBuffer::RecordCommandBuffers(VkCommandBuffer commandBuffer, U32 imageIndex)
    {
        PX_PROFILE_SCOPE("Renderer Prep");

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) 
        {
            PX_CORE_ERROR("Failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = VulkanRenderPass::GetVulkanRenderPass();
        renderPassInfo.framebuffer = VulkanFramebuffer::GetVulkanFramebuffers()[imageIndex];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = VulkanSwapChain::GetVulkanSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { {0.02f, 0.02f, 0.02f, 1.0f} };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<U32>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanPipeline::GetVulkanPipeline());

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)VulkanSwapChain::GetVulkanSwapChainExtent().width;
        viewport.height = (float)VulkanSwapChain::GetVulkanSwapChainExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = VulkanSwapChain::GetVulkanSwapChainExtent();
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
        
        std::vector<VkDescriptorSet> VkDescriptorSets = VulkanUniformBuffer::GetVulkanUniformDescriptorSets();
        VkDescriptorSet VkDescriptorSet = VulkanUniformBuffer::GetVulkanTextureDescriptorSet();

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanPipeline::GetVulkanPipelineLayout(), 0, 1,
            &VkDescriptorSets[VulkanRenderer::GetVulkanCurrentFrame()], 0, nullptr);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanPipeline::GetVulkanPipelineLayout(), 1, 1,
            &VkDescriptorSet, 0, nullptr);
        
        // #### TEMPORARY ####
        if (!AssetManager::GetSelectedMesh().empty())
        {
            VkBuffer& vertexBufferTest = VulkanMesh::GetVulkanMeshNew().vertexBuffer;
            VkDeviceMemory& vertexBufferMemoryTest = VulkanMesh::GetVulkanMeshNew().vertexBufferMemory;
            std::vector<Vertex>& verticesTest = VulkanMesh::GetVulkanMeshNew().vertices;

            VkBuffer& indexBufferTest = VulkanMesh::GetVulkanMeshNew().indexBuffer;
            VkDeviceMemory& indexBufferMemoryTest = VulkanMesh::GetVulkanMeshNew().indexBufferMemory;
            std::vector<U32>& indicesTest = VulkanMesh::GetVulkanMeshNew().indices;

            UpdateCommandBuffers(commandBuffer, vertexBufferTest, indexBufferTest, static_cast<U32>(indicesTest.size()));
        }
        else
        {
            if (VkBuffer& vertexBufferTest = VulkanMesh::GetVulkanMeshNew().vertexBuffer)
            {
                UpdateCommandBuffers(commandBuffer, VulkanMesh::GetVulkanMeshNew().vertexBuffer, VulkanMesh::GetVulkanMeshNew().indexBuffer,
                    static_cast<U32>(VulkanMesh::GetVulkanMeshNew().indices.size()));
            }
            else
            {
                UpdateCommandBuffers(commandBuffer, VulkanMesh::GetVulkanMesh().vertexBuffer, VulkanMesh::GetVulkanMesh().indexBuffer,
                    static_cast<U32>(VulkanMesh::GetVulkanMesh().indices.size()));
            }
        }

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) 
        {
            PX_CORE_ERROR("Failed to record command buffer!");
        }
    }

    void VulkanCommandBuffer::UpdateCommandBuffers(VkCommandBuffer commandBuffer, VkBuffer vertexBuffer, VkBuffer indexBuffer, uint32_t indexCount)
    {
        VkBuffer vertexBuffers[] = { vertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  Command pool
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanCommandBuffer::CreateCommandPool()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_INFO("Creating Vulkan command pool");

        QueueFamilyIndices queueFamilyIndices = m_Device->FindQueueFamilies(VulkanDevice::GetVulkanPhysicalDevice());

        // Command buffers can be very flexible, to avoid doing a
        // lot of allocation while application is trying to render
        // we create a pool to hold allocated command buffers
        //
        // It allows the command buffer to be implicitly reset when vkBeginCommandBuffer is called
        // vkResetCommandBuffer can also explicitly be called 
        //
        // We'll be building command buffers to send to the graphics queue
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO; 
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.m_GraphicsFamily.value();

        if (vkCreateCommandPool(VulkanDevice::GetVulkanDevice(), &poolInfo, nullptr, &s_CommandPool) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create command pool!");
        }
    }

    void VulkanCommandBuffer::DestroyCommandPool()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_WARN("...Destroying Vulkan command pool");

        // Destroys command pool alongside with buffers that are in the pool
        vkDestroyCommandPool(VulkanDevice::GetVulkanDevice(), s_CommandPool, nullptr);
    }
}

