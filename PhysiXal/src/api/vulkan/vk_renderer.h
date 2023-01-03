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
		inline static VkSemaphore s_ImageAvailableSemaphore;
		inline static VkSemaphore s_RenderFinishedSemaphore;
		inline static VkFence s_InFlightFence;
	};
#endif
}