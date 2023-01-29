#include "px_pch.h"
#include "api/vulkan/vk_render_pass.h"

#include "api/vulkan/vk_utilities.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Render pass
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanRenderPass::CreateRenderPass()
    {
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
        VkFormat vkSwapChainImageFormat = VulkanSwapChain::GetVulkanImageFormat();

        PX_CORE_INFO("Creating Vulkan render pass");

        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = vkSwapChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        // Change to VK_IMAGE_LAYOUT_PRESENT_SRC_KHR when multiple render passes are supported and ImGui is implemented
        colorAttachment.finalLayout = /*VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL */ VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

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

        if (vkCreateRenderPass(vkDevice, &renderPassInfo, nullptr, &s_RenderPass) != VK_SUCCESS)
        {
            PX_CORE_ERROR("Failed to create render pass!");
        }
    }

    void VulkanRenderPass::DestroyRenderPass()
    {
        VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

        PX_CORE_WARN("...Destroying Vulkan render pass");

        vkDestroyRenderPass(vkDevice, s_RenderPass, nullptr);
    }
}

