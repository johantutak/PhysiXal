#include "px_pch.h"
#include "api/vulkan/vk_framebuffer.h"

#include "api/vulkan/vk_utilities.h"
#include "api/vulkan/vk_initializers.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Framebuffer
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanFramebuffer::CreateFramebuffers()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_INFO("Creating Vulkan framebuffers");

        s_Framebuffers.resize(VulkanSwapChain::GetVulkanImageViews().size());

        for (size_t i = 0; i < VulkanSwapChain::GetVulkanImageViews().size(); i++) {
            std::array<VkImageView, 3> attachments = {
                VulkanDevice::GetVulkanColorImageView(),
                VulkanDepthBuffer::GetVulkanDepthImageView(),
                VulkanSwapChain::GetVulkanImageViews()[i]
            };

            // VkFrameBuffer maps attachments to a renderpass
            //
            // Needs Vulkans
            //  
            // Render pass
            // Color and depth attachments which is based on size
            // Current render size from the extent
            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = VulkanRenderPass::GetVulkanRenderPass();
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = VulkanSwapChain::GetVulkanSwapChainExtent().width;
            framebufferInfo.height = VulkanSwapChain::GetVulkanSwapChainExtent().height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(VulkanDevice::GetVulkanDevice(), &framebufferInfo, nullptr, &s_Framebuffers[i]) != VK_SUCCESS)
            {
                PX_CORE_ERROR("Failed to create framebuffer!");
            }
        }
    }

    void VulkanFramebuffer::DestroyFramebuffers()
    {
        PX_PROFILE_FUNCTION();

        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_WARN("...Destroying Vulkan framebuffers");

        for (auto framebuffer : s_Framebuffers) 
        {
            vkDestroyFramebuffer(vkDevice, framebuffer, nullptr);
        }
    }
}

