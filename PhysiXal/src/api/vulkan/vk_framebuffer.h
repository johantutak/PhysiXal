#pragma once

#ifdef PX_PLATFORM_WINDOWS
	#include <vulkan/vulkan.h>
#endif

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	class VulkanFramebuffer
	{
	public:
		// Framebuffer
		void CreateFramebuffer();
		void DestroyFramebuffer();

		static std::vector<VkFramebuffer> GetVulkanFramebuffer() { return s_Framebuffer; }
	private:
		static inline std::vector<VkFramebuffer> s_Framebuffer;
	};
#endif
}