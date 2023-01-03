#pragma once

#ifdef PX_PLATFORM_WINDOWS
	#include <vulkan/vulkan.h>
#endif

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

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
#endif
}