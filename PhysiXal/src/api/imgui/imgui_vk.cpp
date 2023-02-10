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
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

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

		if (vkCreateDescriptorPool(vkDevice, &poolInfo, nullptr, &s_GuiDescriptorPool) != VK_SUCCESS)
		{
			PX_CORE_ERROR("Failed to create Dear ImGUI's descriptor pool!");
		}
	}

	void GuiVulkan::DestroyGuiDescriptorPool()
	{
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

		PX_CORE_WARN("...Destroying Dear ImGUI (Vulkan) descriptor pool");

		vkDestroyDescriptorPool(vkDevice, s_GuiDescriptorPool, nullptr);
	}

		// #### Render pass ####
	void GuiVulkan::CreateGuiRenderPass()
	{
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
		VkFormat vkSwapChainImageFormat = VulkanSwapChain::GetVulkanImageFormat();

		PX_CORE_INFO("Creating Dear ImGUI (Vulkan) render pass");

		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = vkSwapChainImageFormat;
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

		if (vkCreateRenderPass(vkDevice, &renderPassInfo, nullptr, &s_GuiRenderPass) != VK_SUCCESS)
		{
			PX_CORE_ERROR("Failed to create Dear ImGUI's render pass!");
		}
	}

	void GuiVulkan::DestroyGuiRenderPass()
	{
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

		PX_CORE_WARN("...Destroying Dear ImGUI (Vulkan) render pass");

		vkDestroyRenderPass(vkDevice, s_GuiRenderPass, nullptr);
	}

		// #### Command buffer ####
	void GuiVulkan::CreateGuiCommandBuffers()
	{
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
		std::vector<VkImageView> vkSwapChainImages = VulkanSwapChain::GetVulkanImageViews();
		VkCommandPool vkCommandPool = VulkanCommandBuffer::GetVulkanCommandPool();

		PX_CORE_INFO("Allocating Dear ImGUI (Vulkan) command buffers");

		s_GuiCommandBuffers.resize(vkSwapChainImages.size());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = s_GuiCommandPool; //vkCommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)s_GuiCommandBuffers.size();

		if (vkAllocateCommandBuffers(vkDevice, &allocInfo, s_GuiCommandBuffers.data()) != VK_SUCCESS)
		{
			PX_CORE_ERROR("Failed to allocate Dear ImGUI's command buffers!");
		}
	}

	void GuiVulkan::DestroyGuiCommandBuffers()
	{
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
		VkCommandPool vkCommandPool = VulkanCommandBuffer::GetVulkanCommandPool();

		PX_CORE_WARN("...Freeing up Dear ImGUI (Vulkan) command buffers");

		vkFreeCommandBuffers(vkDevice, s_GuiCommandPool /*vkCommandPool*/, static_cast<uint32_t>(s_GuiCommandBuffers.size()), s_GuiCommandBuffers.data());
	}

		// #### Command pool ####
	void GuiVulkan::CreateGuiCommandPool()
	{
		VkPhysicalDevice vkPhysicalDevice = VulkanDevice::GetVulkanPhysicalDevice();
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

		PX_CORE_INFO("Creating Dear ImGUI (Vulkan) command pool");

		QueueFamilyIndices queueFamilyIndices = m_Device->FindQueueFamilies(vkPhysicalDevice);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndices.m_GraphicsFamily.value();

		if (vkCreateCommandPool(vkDevice, &poolInfo, nullptr, &s_GuiCommandPool) != VK_SUCCESS)
		{
			PX_CORE_ERROR("Failed to create Dear ImGUI's command pool!");
		}
	}

	void GuiVulkan::DestroyGuiCommandPool()
	{
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

		PX_CORE_WARN("...Destroying Dear ImGUI (Vulkan) command pool");

		// Destroys command buffers that are in the pool
		vkDestroyCommandPool(vkDevice, s_GuiCommandPool, nullptr);
	}

		// #### Framebuffer ####
	void GuiVulkan::CreateGuiFramebuffers()
	{
		std::vector<VkImageView> vkSwapChainImageViews = VulkanSwapChain::GetVulkanImageViews();
		VkExtent2D vkSwapChainExtent2D = VulkanSwapChain::GetVulkanSwapChainExtent();
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
		VkImageView vkColorImageView = VulkanDevice::GetVulkanColorImageView();

		PX_CORE_INFO("Creating Dear ImGUI (Vulkan) framebuffers");

		for (size_t i = 0; i < vkSwapChainImageViews.size(); i++) {
			std::array<VkImageView, 1> attachment = {
			vkSwapChainImageViews[i]
			};

			s_GuiFramebuffers.resize(vkSwapChainImageViews.size());

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = s_GuiRenderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachment.data();
			framebufferInfo.width = vkSwapChainExtent2D.width;
			framebufferInfo.height = vkSwapChainExtent2D.height;
			framebufferInfo.layers = 1;


			if (vkCreateFramebuffer(vkDevice, &framebufferInfo, nullptr, &s_GuiFramebuffers[i]) != VK_SUCCESS)
			{
				PX_CORE_ERROR("Failed to create Dear ImGUI's framebuffer!");
			}
		}
	}

	void GuiVulkan::DestroyGuiFramebuffers()
	{
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

		PX_CORE_WARN("...Destroying Dear ImGUI (Vulkan) framebuffers");

		for (auto framebuffer : s_GuiFramebuffers)
		{
			vkDestroyFramebuffer(vkDevice, framebuffer, nullptr);
		}
	}
}