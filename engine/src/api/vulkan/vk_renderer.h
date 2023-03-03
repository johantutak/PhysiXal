#pragma once

#include <vulkan/vulkan.h>

#include "scene/camera.h"

namespace PhysiXal {

	// #### TEMPORARY ####
	const std::string MODEL_PATH = "../editor/assets/models/viking_room.obj";
	const std::string TEXTURE_PATH = "../editor/assets/textures/viking_room.png";

	// #### TO DO #### Framebuffer reset in window fix and fix renderer destruction order

	class VulkanRenderer
	{
	public:
		// Vulkan renderer
		void Init();
		void Shutdown();

		void DrawFrame();

		void WaitAndIdle();

		//void SetCamera(Camera* camera);

		static uint32_t GetVulkanCurrentFrame() { return s_CurrentFrame; }
	private:
		Camera* m_Camera = nullptr;
	private:
		inline static uint32_t s_CurrentFrame = 0;

		inline static bool s_FramebufferResized;
	};
}