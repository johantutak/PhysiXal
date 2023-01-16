#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

	class VulkanRenderer
	{
	public:
		void Init();
		void Shutdown();

		void BeginFrame();

		void WaitAndIdle();

		void CreateSyncObjects();
		void DestroySyncObjects();
	private:
		inline static std::vector<VkSemaphore> s_ImageAvailableSemaphores;
		inline static std::vector<VkSemaphore> s_RenderFinishedSemaphores;
		inline static std::vector<VkFence> s_InFlightFences;

		inline static uint32_t s_CurrentFrame = 0;
	};
}