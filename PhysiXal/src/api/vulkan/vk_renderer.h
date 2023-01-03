#pragma once

#ifdef PX_PLATFORM_WINDOWS
	#include <vulkan/vulkan.h>
#endif

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

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
		VkSemaphore m_ImageAvailableSemaphore;
		VkSemaphore m_RenderFinishedSemaphore;
		VkFence m_InFlightFence;
	};
#endif
}