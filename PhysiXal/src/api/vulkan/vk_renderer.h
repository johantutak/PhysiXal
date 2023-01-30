#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

	class VulkanRenderer
	{
	public:
		// Vulkan renderer
		void Init();
		void Shutdown();

		void BeginFrame();

		void WaitAndIdle();

		static uint32_t GetVulkanCurrentFrame() { return s_CurrentFrame; }
	private:
		inline static uint32_t s_CurrentFrame = 0;

		inline static bool s_FramebufferResized;
	};
}