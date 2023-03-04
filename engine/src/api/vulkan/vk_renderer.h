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

		void BeginFrame();
		void EndFrame();

		void WaitAndIdle();

		// #### TO DO #### Camera class works but needs to get implemented to work with the renderer and it's scene
		//void SetCamera(Camera* camera);

		static uint32_t GetVulkanCurrentFrame() { return s_CurrentFrame; }

		static uint32_t GetVulkanImageIndex() { return s_ImageIndex; }
	private:
		Camera* m_Camera = nullptr;
	private:
		inline static uint32_t s_CurrentFrame = 0;

		inline static uint32_t s_ImageIndex;

		inline static bool s_FramebufferResized;
	};
}