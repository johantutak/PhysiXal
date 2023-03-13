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

		static U32 GetVulkanCurrentFrame() { return s_CurrentFrame; }

		static U32 GetVulkanImageIndex() { return s_ImageIndex; }
	private:
		Camera* m_Camera = nullptr;
	private:
		inline static U32 s_CurrentFrame = 0;

		inline static U32 s_ImageIndex;

		inline static bool s_FramebufferResized;
	};
}