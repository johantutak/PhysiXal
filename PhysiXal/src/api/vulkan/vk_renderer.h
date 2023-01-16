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

		void RecreateSwapChain();
		void DestroyRecreatedSwapChain();
	private:
		inline static uint32_t s_CurrentFrame = 0;

		inline static bool s_FramebufferResized;
	};
}