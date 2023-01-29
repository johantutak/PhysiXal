#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

	class VulkanTexture
	{
	public:
		// Texture
		void CreateTextureImage();
		void DestroyTextureImage();
		void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, 
			VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

		//Texture image view
		void CreateTextureImageView();
		void DestroyTextureImageView();

		static VkImageView GetVulkanTextureImageView() { return s_TextureImageView; }

		// Texture sampler
		void CreateTextureSampler();
		void DestroyTextureSampler();

		static VkSampler GetVulkanTextureSampler() { return s_TextureSampler; }
	private:
		inline static VkImage s_TextureImage;
		inline static VkDeviceMemory s_TextureImageMemory;

		inline static VkImageView s_TextureImageView;

		inline static VkSampler s_TextureSampler;
	};
}