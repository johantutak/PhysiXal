#include "px_pch.h"
#include "api/vulkan/vk_sync_objects.h"

#include "api/vulkan/vk_device.h"
#include "api/vulkan/vk_command_buffer.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

	void VulkanSyncObjects::CreateSyncObjects()
	{
		PX_CORE_INFO("Creating sync objects");

		s_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		s_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		s_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
			if (vkCreateSemaphore(vkDevice, &semaphoreInfo, nullptr, &s_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(vkDevice, &semaphoreInfo, nullptr, &s_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(vkDevice, &fenceInfo, nullptr, &s_InFlightFences[i]) != VK_SUCCESS)
			{
				PX_CORE_ERROR("Failed to create synchronization objects for a frame!");
			}
		}
	}

	void VulkanSyncObjects::DestroySyncObjects()
	{
		PX_CORE_WARN("...Destroying sync objects");

		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(vkDevice, s_ImageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(vkDevice, s_RenderFinishedSemaphores[i], nullptr);
			vkDestroyFence(vkDevice, s_InFlightFences[i], nullptr);
		}
	}
}