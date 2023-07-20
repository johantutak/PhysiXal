#pragma once

#include <vulkan/vulkan.h>

#include <stb_image.h>

namespace PhysiXal {

	struct Texture {
		VkImage textureImage;
		VkDeviceMemory textureImageMemory;

		VkImageView textureImageView;

		VkSampler textureSampler;
	};

	class VulkanTexture
	{
	public:
		// Texture
		void CreateTextureImage(VkImage& textureImage, VkDeviceMemory& textureImageMemory, const std::string& filePath);
		void DestroyTextureImage(VkImage& textureImage, VkDeviceMemory& textureImageMemory);
		stbi_uc* LoadTextureData(const std::string& filePath, int& texWidth, int& texHeight, int& texChannels);
		void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, U32 mipLevels);
		void CopyBufferToImage(VkBuffer buffer, VkImage image, U32 width, U32 height);

		static Texture& GetVulkanTexture() { return texture; }
		//static Texture& GetVulkanTextureNew() { return textureNew; }

		//Texture image view
		void CreateTextureImageView(VkImage& textureImage, VkImageView& textureImageView);
		void DestroyTextureImageView(VkImageView& textureImageView);

		// Texture sampler
		void CreateTextureSampler(VkSampler& textureSampler);
		void DestroyTextureSampler(VkSampler& textureSampler);

		// Mipmaps
		void GenerateMipmaps(VkImage image, VkFormat imageFormat, I32 texWidth, I32 texHeight, U32 mipLevels);

		static U32 GetVulkanMipLevels() { return s_MipLevels; }
	private:
		inline static Texture texture;
		//inline static Texture textureNew;

		inline static U32 s_MipLevels;
	};
}