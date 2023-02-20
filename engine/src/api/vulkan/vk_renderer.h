#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

	// #### TEMPORARY ####
#ifdef PX_PLATFORM_WINDOWS
	const std::string MODEL_PATH = "../editor/assets/models/viking_room.obj";
	const std::string TEXTURE_PATH = "../editor/assets/textures/viking_room.png";
#endif
#ifdef PX_PLATFORM_LINUX
	const std::string MODEL_PATH = "../../../editor/assets/models/viking_room.obj";
	const std::string TEXTURE_PATH = "../../../editor/assets/textures/viking_room.png";
#endif

	// #### TO DO #### Framebuffer reset in window fix and fix renderer destruction order

	class VulkanRenderer
	{
	public:
		// Vulkan renderer
		void Init();
		void Shutdown();

		void DrawFrame();

		void WaitAndIdle();

		static uint32_t GetVulkanCurrentFrame() { return s_CurrentFrame; }
	private:
		inline static uint32_t s_CurrentFrame = 0;

		inline static bool s_FramebufferResized;
	};
}