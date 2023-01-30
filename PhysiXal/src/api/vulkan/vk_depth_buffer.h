#pragma once

#include <glm/glm.hpp>

#include <vulkan/vulkan.h>

namespace PhysiXal {

	class VulkanDepthBuffer
	{
	public:
		// Depth buffer
		void CreateDepthResources();
		void DestroyDepthResources();

		bool HasStencilComponent(VkFormat format);

		static VkImageView GetVulkanDepthImageView() { return s_DepthImageView; }
	private:
		static inline VkImage s_DepthImage;
		static inline VkDeviceMemory s_DepthImageMemory;
		static inline VkImageView s_DepthImageView;
	};
}
