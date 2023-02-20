#include "px_pch.h"
#include "vk_depth_buffer.h"

#include "api/vulkan/vk_utilities.h"
#include "api/vulkan/vk_initializers.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Depth buffer
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void VulkanDepthBuffer::CreateDepthResources()
	{
		PX_PROFILE_FUNCTION();

		VkExtent2D vkSwapChainExtent2D = VulkanSwapChain::GetVulkanSwapChainExtent();
		VkSampleCountFlagBits vkMsaaSamples = VulkanDevice::GetVulkanMsaa();

		PX_CORE_INFO("Creating Vulkan depth buffer");

		VkFormat depthFormat = FindDepthFormat();

		CreateImage(vkSwapChainExtent2D.width, vkSwapChainExtent2D.height, 1, vkMsaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, s_DepthImage, s_DepthImageMemory);
		s_DepthImageView = CreateImageView(s_DepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
	}

	void VulkanDepthBuffer::DestroyDepthResources()
	{
		PX_PROFILE_FUNCTION();

		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

		PX_CORE_WARN("...Destroying Vulkan depth buffer");

		vkDestroyImageView(vkDevice, s_DepthImageView, nullptr);
		vkDestroyImage(vkDevice, s_DepthImage, nullptr);
		vkFreeMemory(vkDevice, s_DepthImageMemory, nullptr);
	}

	bool VulkanDepthBuffer::HasStencilComponent(VkFormat format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}
}