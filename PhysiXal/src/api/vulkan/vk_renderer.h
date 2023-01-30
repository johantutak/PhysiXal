#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS
	const std::string MODEL_PATH = "../Example/assets/models/viking_room.obj";
	const std::string TEXTURE_PATH = "../Example/assets/textures/viking_room.png";
#endif
#ifdef PX_PLATFORM_LINUX
	const std::string MODEL_PATH = "../../../Example/assets/models/viking_room.obj";
	const std::string TEXTURE_PATH = "../../../Example/assets/textures/viking_room.png";
#endif

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