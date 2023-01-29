#include "px_pch.h"
#include "api/vulkan/vk_framebuffer.h"

#include "api/vulkan/vk_utilities.h"

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

        PX_CORE_INFO("Creating Vulkan framebuffer");

        s_Framebuffers.resize(vkSwapChainImageViews.size());

        for (size_t i = 0; i < vkSwapChainImageViews.size(); i++)
        {
            VkImageView attachments[] =
            {
                vkSwapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = vkRenderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
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

        PX_CORE_WARN("...Destroying Vulkan framebuffer");

        for (auto framebuffer : s_Framebuffers) 
        {
            vkDestroyFramebuffer(vkDevice, framebuffer, nullptr);
        }
    }
}

