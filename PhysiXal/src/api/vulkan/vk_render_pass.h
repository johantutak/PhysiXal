#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

	class VulkanRenderPass
	{
	public:
		// Render pass
		void CreateRenderPass();
		void DestroyRenderPass();

		static VkRenderPass GetVulkanRenderPass() { return s_RenderPass; }
	private:
		inline static VkRenderPass s_RenderPass;
	};
}