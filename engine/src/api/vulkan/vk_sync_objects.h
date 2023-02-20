#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

	class VulkanSyncObjects
	{
	public:
		// Sync objects
		void CreateSyncObjects();
		void DestroySyncObjects();

		static std::vector<VkSemaphore> GetVulkanImageSemaphores() { return s_ImageAvailableSemaphores; }
		static std::vector<VkSemaphore> GetVulkanRenderSemaphores() { return s_RenderFinishedSemaphores; }
		static std::vector<VkFence> GetVulkanFences() { return s_InFlightFences; }
	private:
		inline static std::vector<VkSemaphore> s_ImageAvailableSemaphores;
		inline static std::vector<VkSemaphore> s_RenderFinishedSemaphores;
		inline static std::vector<VkFence> s_InFlightFences;
	};
}