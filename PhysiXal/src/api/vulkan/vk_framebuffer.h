#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

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
}