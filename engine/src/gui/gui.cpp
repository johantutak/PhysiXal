#include "px_pch.h"
#include "gui/gui.h"

#include "api/imgui/imgui_initializers.h"
#include "api/imgui/imgui_layer.h"

#include "api/vulkan/vk_initializers.h"

namespace PhysiXal {

	static GuiLayer* s_Gui = nullptr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// GUI
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Gui::Init()
	{
		PX_PROFILE_FUNCTION();

		s_Gui->GuiInit();
	}

	void Gui::Shutdown()
	{
		PX_PROFILE_FUNCTION();

		s_Gui->GuiShutdown();
	}

	void Gui::Begin()
	{
		PX_PROFILE_FUNCTION();

		s_Gui->GuiBegin();
	}

	void Gui::End()
	{
		PX_PROFILE_FUNCTION();

		s_Gui->GuiEnd();
	}

	void Gui::Draw()
	{
		PX_PROFILE_FUNCTION();

		s_Gui->GuiDraw(GuiVulkan::GetGuiCommandBuffers()[VulkanRenderer::GetVulkanCurrentFrame()], VulkanRenderer::GetVulkanImageIndex());
	}

	void Gui::OnRender()
	{
		PX_PROFILE_FUNCTION();

		s_Gui->GuiOnRender();
	}

	void Gui::OnRebuild()
	{
		PX_PROFILE_FUNCTION();

		s_Gui->GuiOnRebuild();
	}
}