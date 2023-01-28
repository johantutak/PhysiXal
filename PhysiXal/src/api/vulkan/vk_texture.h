#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {



	class VulkanTexture
	{
	public:
		// Texture
		void ApplyTextureImage();
		void EraseTextureImage();

		void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, 
			VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

		void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	private:
		inline static VkImage s_TextureImage;
		inline static VkDeviceMemory s_TextureImageMemory;
	};
}