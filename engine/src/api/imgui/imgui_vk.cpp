#include "px_pch.h"
#include "api/imgui/imgui_vk.h"

#include "api/vulkan/vk_initializers.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Vulkan (Dear ImGUI)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// #### Descriptor pool ####
	void GuiVulkan::CreateGuiDescriptorPool()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_INFO("Creating Dear ImGUI (Vulkan) descriptor pool");

		VkDescriptorPoolSize poolSizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 100 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 100 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 100 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 100 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 100 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 100 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 100 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 100 }
		};

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.maxSets = 100;
		poolInfo.poolSizeCount = std::size(poolSizes);
		poolInfo.pPoolSizes = poolSizes;

		if (vkCreateDescriptorPool(VulkanDevice::GetVulkanDevice(), &poolInfo, nullptr, &s_GuiDescriptorPool) != VK_SUCCESS)
		{
			PX_CORE_ERROR("Failed to create Dear ImGUI's descriptor pool!");
		}
	}

	void GuiVulkan::DestroyGuiDescriptorPool()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_WARN("...Destroying Dear ImGUI (Vulkan) descriptor pool");

		vkDestroyDescriptorPool(VulkanDevice::GetVulkanDevice(), s_GuiDescriptorPool, nullptr);
	}

		// #### Render pass ####
	void GuiVulkan::CreateGuiRenderPass()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_INFO("Creating Dear ImGUI (Vulkan) render pass");

		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = VulkanSwapChain::GetVulkanImageFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;  // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(VulkanDevice::GetVulkanDevice(), &renderPassInfo, nullptr, &s_GuiRenderPass) != VK_SUCCESS)
		{
			PX_CORE_ERROR("Failed to create Dear ImGUI's render pass!");
		}
	}

	void GuiVulkan::DestroyGuiRenderPass()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_WARN("...Destroying Dear ImGUI (Vulkan) render pass");

		vkDestroyRenderPass(VulkanDevice::GetVulkanDevice(), s_GuiRenderPass, nullptr);
	}

		// #### Command buffer ####
	void GuiVulkan::CreateGuiCommandBuffers()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_INFO("Allocating Dear ImGUI (Vulkan) command buffers");

		s_GuiCommandBuffers.resize(VulkanSwapChain::GetVulkanImageViews().size());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = s_GuiCommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (U32)s_GuiCommandBuffers.size();

		if (vkAllocateCommandBuffers(VulkanDevice::GetVulkanDevice(), &allocInfo, s_GuiCommandBuffers.data()) != VK_SUCCESS)
		{
			PX_CORE_ERROR("Failed to allocate Dear ImGUI's command buffers!");
		}
	}

	void GuiVulkan::DestroyGuiCommandBuffers()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_WARN("...Freeing up Dear ImGUI (Vulkan) command buffers");

		vkFreeCommandBuffers(VulkanDevice::GetVulkanDevice(), s_GuiCommandPool, static_cast<U32>(s_GuiCommandBuffers.size()), s_GuiCommandBuffers.data());
	}

		// #### Command pool ####
	void GuiVulkan::CreateGuiCommandPool()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_INFO("Creating Dear ImGUI (Vulkan) command pool");

		QueueFamilyIndices queueFamilyIndices = m_Device->FindQueueFamilies(VulkanDevice::GetVulkanPhysicalDevice());

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndices.m_GraphicsFamily.value();

		if (vkCreateCommandPool(VulkanDevice::GetVulkanDevice(), &poolInfo, nullptr, &s_GuiCommandPool) != VK_SUCCESS)
		{
			PX_CORE_ERROR("Failed to create Dear ImGUI's command pool!");
		}
	}

	void GuiVulkan::DestroyGuiCommandPool()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_WARN("...Destroying Dear ImGUI (Vulkan) command pool");

		// Destroys command buffers that are in the pool
		vkDestroyCommandPool(VulkanDevice::GetVulkanDevice(), s_GuiCommandPool, nullptr);
	}

		// #### Framebuffer ####
	void GuiVulkan::CreateGuiFramebuffers()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_INFO("Creating Dear ImGUI (Vulkan) framebuffers");

		for (SIZE64 i = 0; i < VulkanSwapChain::GetVulkanImageViews().size(); i++) {
			std::array<VkImageView, 1> attachment = {
			VulkanSwapChain::GetVulkanImageViews()[i]
			};

			s_GuiFramebuffers.resize(VulkanSwapChain::GetVulkanImageViews().size());

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = s_GuiRenderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachment.data();
			framebufferInfo.width = VulkanSwapChain::GetVulkanSwapChainExtent().width;
			framebufferInfo.height = VulkanSwapChain::GetVulkanSwapChainExtent().height;
			framebufferInfo.layers = 1;


			if (vkCreateFramebuffer(VulkanDevice::GetVulkanDevice(), &framebufferInfo, nullptr, &s_GuiFramebuffers[i]) != VK_SUCCESS)
			{
				PX_CORE_ERROR("Failed to create Dear ImGUI's framebuffer!");
			}
		}
	}

	void GuiVulkan::DestroyGuiFramebuffers()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_WARN("...Destroying Dear ImGUI (Vulkan) framebuffers");

		for (auto framebuffer : s_GuiFramebuffers)
		{
			vkDestroyFramebuffer(VulkanDevice::GetVulkanDevice(), framebuffer, nullptr);
		}
	}
}