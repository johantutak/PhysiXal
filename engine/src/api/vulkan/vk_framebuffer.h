#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

	class VulkanFramebuffer
	{
	public:
		// Framebuffer
		void CreateFramebuffers();
		void DestroyFramebuffers();

		static std::vector<VkFramebuffer> GetVulkanFramebuffers() { return s_Framebuffers; }
	private:
		static inline std::vector<VkFramebuffer> s_Framebuffers;
	};
}