#include "px_pch.h"
#include "api/vulkan/vk_sync_objects.h"

#include "api/vulkan/vk_utilities.h"
#include "api/vulkan/vk_initializers.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Sync objects
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void VulkanSyncObjects::CreateSyncObjects()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_INFO("Creating sync objects");

		s_ImageAvailableSemaphores.resize(c_MaxImageCount);
		s_RenderFinishedSemaphores.resize(c_MaxImageCount);
		s_InFlightFences.resize(c_MaxImageCount);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < c_MaxImageCount; i++)
		{
			if (vkCreateSemaphore(VulkanDevice::GetVulkanDevice(), &semaphoreInfo, nullptr, &s_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(VulkanDevice::GetVulkanDevice(), &semaphoreInfo, nullptr, &s_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(VulkanDevice::GetVulkanDevice(), &fenceInfo, nullptr, &s_InFlightFences[i]) != VK_SUCCESS)
			{
				PX_CORE_ERROR("Failed to create synchronization objects for a frame!");
			}
		}
	}

	void VulkanSyncObjects::DestroySyncObjects()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_WARN("...Destroying sync objects");

		for (size_t i = 0; i < c_MaxImageCount; i++)
		{
			vkDestroySemaphore(VulkanDevice::GetVulkanDevice(), s_ImageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(VulkanDevice::GetVulkanDevice(), s_RenderFinishedSemaphores[i], nullptr);
			vkDestroyFence(VulkanDevice::GetVulkanDevice(), s_InFlightFences[i], nullptr);
		}
	}
}