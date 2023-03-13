#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

	class VulkanTexture
	{
	public:
		// Texture
		void CreateTextureImage();
		void DestroyTextureImage();
		void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, U32 mipLevels);
		void CopyBufferToImage(VkBuffer buffer, VkImage image, U32 width, U32 height);

		//Texture image view
		void CreateTextureImageView();
		void DestroyTextureImageView();

		static VkImageView GetVulkanTextureImageView() { return s_TextureImageView; }

		// Texture sampler
		void CreateTextureSampler();
		void DestroyTextureSampler();

		static VkSampler GetVulkanTextureSampler() { return s_TextureSampler; }

		// Mipmaps
		void GenerateMipmaps(VkImage image, VkFormat imageFormat, I32 texWidth, I32 texHeight, U32 mipLevels);
	private:
		inline static VkImage s_TextureImage;
		inline static VkDeviceMemory s_TextureImageMemory;

		inline static VkImageView s_TextureImageView;

		inline static VkSampler s_TextureSampler;

		inline static uint32_t s_MipLevels;
	};
}