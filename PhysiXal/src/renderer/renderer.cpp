#include "px_pch.h"
#include "renderer/renderer.h"

#include <api/vulkan/vk_renderer.h>

namespace PhysiXal {
	
	static VulkanRenderer* s_Renderer = nullptr;

	void Renderer::Init()
	{
		PX_PROFILE_FUNCTION();

		s_Renderer->Init();
	}

	void Renderer::Shutdown()
	{
		PX_PROFILE_FUNCTION();

		s_Renderer->Shutdown();
	}

	void Renderer::DrawFrame()
	{
		PX_PROFILE_FUNCTION();

		s_Renderer->DrawFrame();
	}

	void Renderer::WaitAndIdle()
	{
		PX_PROFILE_FUNCTION();

		s_Renderer->WaitAndIdle();
	}
}