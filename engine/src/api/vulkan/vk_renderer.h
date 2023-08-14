#pragma once

#include <vulkan/vulkan.h>

#include "scene/camera.h"

#include "api/imgui/imgui_widgets.h"

namespace PhysiXal {

	// #### TO DO #### Framebuffer reset in window fix and fix renderer destruction order

	class VulkanRenderer
	{
	public:
		// Vulkan renderer
		void Init();
		void Shutdown();

		void BeginFrame();
		void EndFrame();

		void WaitAndIdle();

		static U32 GetVulkanCurrentFrame() { return s_CurrentFrame; }

		static U32 GetVulkanImageIndex() { return s_ImageIndex; }
	private:
		inline static U32 s_CurrentFrame = 0;

		inline static U32 s_ImageIndex;

		inline static bool s_FramebufferResized;
	};
}