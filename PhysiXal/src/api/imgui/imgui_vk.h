#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

	class GuiVulkan
	{
	public:
		// Vulkan (Dear ImGUI)

			// Descriptor pool
		void CreateGuiDescriptorPool();
		void DestroyGuiDescriptorPool();

		static VkDescriptorPool GetGuiDescriptorPool() { return s_GuiDescriptorPool; }

			// Render pass
		void CreateGuiRenderPass();
		void DestroyGuiRenderPass();

		static VkRenderPass GetGuiRenderPass() { return s_GuiRenderPass; }

			// Command buffer
		void CreateGuiCommandBuffers();
		void DestroyGuiCommandBuffers();

		static std::vector<VkCommandBuffer> GetGuiCommandBuffers() { return s_GuiCommandBuffers; }

			// Command pool
		void CreateGuiCommandPool();
		void DestroyGuiCommandPool();

		static VkCommandPool GetGuiCommandPool() { return s_GuiCommandPool; }

			// Framebuffer
		void CreateGuiFramebuffers();
		void DestroyGuiFramebuffers();

		static std::vector<VkFramebuffer> GetGuiFramebuffers() { return s_GuiFramebuffers; }
	private:
		inline static VkDescriptorPool s_GuiDescriptorPool;

		inline static VkRenderPass s_GuiRenderPass;

		inline static std::vector<VkCommandBuffer> s_GuiCommandBuffers;
		inline static VkCommandPool s_GuiCommandPool;

		inline static std::vector<VkFramebuffer> s_GuiFramebuffers;
	};
}