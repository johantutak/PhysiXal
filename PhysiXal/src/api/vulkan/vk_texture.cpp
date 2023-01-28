#include "px_pch.h"
#include "api/vulkan/vk_texture.h"

#include "api/vulkan/vk_device.h"
#include "api/vulkan/vk_buffer.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stb_image.h>

namespace PhysiXal {

	static VulkanBuffer* m_Buffer = nullptr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Proxy classes
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	VkImageView CreateImageView(VkImage image, VkFormat format) 
	{
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;
		if (vkCreateImageView(vkDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) 
		{
			PX_CORE_ERROR("Failed to create texture image view!");
		}

		return imageView;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Texture
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void VulkanTexture::CreateTextureImage()
	{
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

		PX_CORE_INFO("Applying texture image");

		int texWidth, texHeight, texChannels;
#ifdef PX_PLATFORM_WINDOWS
		stbi_uc* pixels = stbi_load("../Example/assets/textures/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
#endif
#ifdef PX_PLATFORM_LINUX
		stbi_uc* pixels = stbi_load("../../../Example/assets/textures/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
#endif
		VkDeviceSize imageSize = texWidth * texHeight * 4;

		if (!pixels) 
		{
			PX_CORE_ERROR("Failed to load texture image!");
		}

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		m_Buffer->CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(vkDevice, stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(vkDevice, stagingBufferMemory);

		stbi_image_free(pixels);
		
		CreateImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, s_TextureImage, s_TextureImageMemory);

		TransitionImageLayout(s_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		CopyBufferToImage(stagingBuffer, s_TextureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
		TransitionImageLayout(s_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		vkDestroyBuffer(vkDevice, stagingBuffer, nullptr);
		vkFreeMemory(vkDevice, stagingBufferMemory, nullptr);
	}

	void VulkanTexture::DestroyTextureImage()
	{
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

		PX_CORE_WARN("...Erasing texture image");

		vkDestroyImage(vkDevice, s_TextureImage, nullptr);
		vkFreeMemory(vkDevice, s_TextureImageMemory, nullptr);
	}

	void VulkanTexture::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, 
		VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) 
	{
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(vkDevice, &imageInfo, nullptr, &image) != VK_SUCCESS)
		{
			PX_CORE_ERROR("Failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(vkDevice, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = m_Buffer->FindMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(vkDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) 
		{
			PX_CORE_ERROR("Failed to allocate image memory!");
		}

		vkBindImageMemory(vkDevice, image, imageMemory, 0);
	}

	void VulkanTexture::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else 
		{
			PX_CORE_ERROR("Unsupported layout transition!");
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		EndSingleTimeCommands(commandBuffer);
	}

	void VulkanTexture::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width,
			height,
			1
		};

		vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		EndSingleTimeCommands(commandBuffer);
	}

	void VulkanTexture::CreateTextureImageView()
	{
		PX_CORE_INFO("Setting up and creating Vulkan texture image views");

		s_TextureImageView = CreateImageView(s_TextureImage, VK_FORMAT_R8G8B8A8_SRGB);
	}

	void VulkanTexture::DestroyTextureImageView()
	{
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

		PX_CORE_WARN("...Destroying Vulkan texture image views");

		vkDestroyImage(vkDevice, s_TextureImage, nullptr);
		vkFreeMemory(vkDevice, s_TextureImageMemory, nullptr);
	}

	void VulkanTexture::CreateTextureSampler()
	{
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
		VkPhysicalDevice vkPhysicalDevice = VulkanDevice::GetVulkanPhysicalDevice();

		PX_CORE_INFO("Creating Vulkan texture sampler");

		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(vkPhysicalDevice, &properties);

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

		if (vkCreateSampler(vkDevice, &samplerInfo, nullptr, &s_TextureSampler) != VK_SUCCESS)
		{
			PX_CORE_ERROR("Failed to create texture sampler!");
		}
	}

	void VulkanTexture::DestroyTextureSampler()
	{
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

		PX_CORE_WARN("...Destroying Vulkan texture sampler");

		vkDestroySampler(vkDevice, s_TextureSampler, nullptr);
	}
}