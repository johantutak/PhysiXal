#include "px_pch.h"
#include "api/vulkan/vk_render_pass.h"

#include "api/vulkan/vk_device.h"
#include "api/vulkan/vk_swap_chain.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

    void VulkanRenderPass::CreateRenderPass()
    {
        PX_CORE_INFO("Creating Vulkan context");

        VkAttachmentDescription colorAttachment{};
        VkFormat vkSwapChainImageFormat = VulkanSwapChain::GetVulkanImageFormat();
        colorAttachment.format = vkSwapChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
        if (vkCreateRenderPass(vkDevice, &renderPassInfo, nullptr, &s_RenderPass) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create render pass!");
        }
    }

    void VulkanRenderPass::DestroyRenderPass()
    {
        PX_CORE_WARN("...Destroying Vulkan context");

        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
        vkDestroyRenderPass(vkDevice, s_RenderPass, nullptr);
    }
#endif
}

