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
        std::vector<VkImageView> vkSwapChainImageViews = VulkanSwapChain::GetVulkanImageViews();
        VkRenderPass vkRenderPass = VulkanRenderPass::GetVulkanRenderPass();
        VkExtent2D vkSwapChainExtent2D = VulkanSwapChain::GetVulkanSwapChainExtent();
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
        VkImageView vkColorImageView = VulkanDevice::GetVulkanColorImageView();
        VkImageView vkDepthImageView = VulkanDepthBuffer::GetVulkanDepthImageView();

        PX_CORE_INFO("Creating Vulkan framebuffers");

        s_Framebuffers.resize(vkSwapChainImageViews.size());

        for (size_t i = 0; i < vkSwapChainImageViews.size(); i++) {
            std::array<VkImageView, 3> attachments = {
                vkColorImageView,
                vkDepthImageView,
                vkSwapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = vkRenderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = vkSwapChainExtent2D.width;
            framebufferInfo.height = vkSwapChainExtent2D.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(vkDevice, &framebufferInfo, nullptr, &s_Framebuffers[i]) != VK_SUCCESS)
            {
                PX_CORE_ERROR("Failed to create framebuffer!");
            }
        }
    }

    void VulkanFramebuffer::DestroyFramebuffers()
    {
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_WARN("...Destroying Vulkan framebuffers");

        for (auto framebuffer : s_Framebuffers) 
        {
            vkDestroyFramebuffer(vkDevice, framebuffer, nullptr);
        }
    }
}

