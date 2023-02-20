#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

	class VulkanTexture
	{
	public:
		// Texture
		void CreateTextureImage();
		void DestroyTextureImage();
		void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

		//Texture image view
		void CreateTextureImageView();
		void DestroyTextureImageView();

		static VkImageView GetVulkanTextureImageView() { return s_TextureImageView; }

		// Texture sampler
		void CreateTextureSampler();
		void DestroyTextureSampler();

		static VkSampler GetVulkanTextureSampler() { return s_TextureSampler; }

		// Mipmaps
		void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	private:
		inline static VkImage s_TextureImage;
		inline static VkDeviceMemory s_TextureImageMemory;

		inline static VkImageView s_TextureImageView;

		inline static VkSampler s_TextureSampler;

		inline static uint32_t s_MipLevels;
	};
}