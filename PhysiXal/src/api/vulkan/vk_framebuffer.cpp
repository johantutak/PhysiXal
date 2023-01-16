#include "px_pch.h"
#include "api/vulkan/vk_framebuffer.h"

#include "api/vulkan/vk_device.h"
#include "api/vulkan/vk_swap_chain.h"
#include "api/vulkan/vk_render_pass.h"
#include "api/vulkan/vk_pipeline.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

    void VulkanFramebuffer::CreateFramebuffer()
    {
        PX_CORE_INFO("Creating Vulkan framebuffer");
        
        std::vector<VkImageView> vkSwapChainImageViews = VulkanSwapChain::GetVulkanImageViews();
        s_Framebuffer.resize(vkSwapChainImageViews.size());

        for (size_t i = 0; i < vkSwapChainImageViews.size(); i++)
        {
            VkImageView attachments[] =
            {
                vkSwapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            
            VkRenderPass vkRenderPass = VulkanRenderPass::GetVulkanRenderPass();
            framebufferInfo.renderPass = vkRenderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            
            VkExtent2D vkSwapChainExtent2D = VulkanSwapChain::GetVulkanSwapChainExtent();
            framebufferInfo.width = vkSwapChainExtent2D.width;
            framebufferInfo.height = vkSwapChainExtent2D.height;
            framebufferInfo.layers = 1;

            VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
            if (vkCreateFramebuffer(vkDevice, &framebufferInfo, nullptr, &s_Framebuffer[i]) != VK_SUCCESS)
            {
                PX_CORE_ERROR("Failed to create framebuffer!");
            }
        }
    }

    void VulkanFramebuffer::DestroyFramebuffer()
    {
        PX_CORE_WARN("...Destroying Vulkan framebuffer");

        for (auto framebuffer : s_Framebuffer) 
        {
            VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
            vkDestroyFramebuffer(vkDevice, framebuffer, nullptr);
        }
    }
}

