#include "px_pch.h"
#include "renderer/renderer.h"

#include <api/vulkan/vk_renderer.h>

namespace PhysiXal {
	
	static VulkanRenderer* s_Renderer = nullptr;

	void Renderer::Init()
	{
		s_Renderer->Init();
	}

	void Renderer::Shutdown()
	{
		s_Renderer->Shutdown();
	}

	void Renderer::DrawFrame()
	{
		s_Renderer->DrawFrame();
	}

	void Renderer::WaitAndIdle()
	{
		s_Renderer->WaitAndIdle();
	}
}