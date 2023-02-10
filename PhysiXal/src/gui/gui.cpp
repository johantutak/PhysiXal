#include "px_pch.h"
#include "gui/gui.h"

#include "api/imgui/imgui_layer.h"

namespace PhysiXal {

	static GuiLayer* s_Gui = nullptr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// GUI
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Gui::Init()
	{
		s_Gui->GuiInit();
	}

	void Gui::Shutdown()
	{
		s_Gui->GuiShutdown();
	}

	void Gui::Begin()
	{
		s_Gui->GuiBegin();
	}

	void Gui::End()
	{
		s_Gui->GuiEnd();
	}

	/*void Gui::Draw(VkCommandBuffer commandBuffer, uint32_t imageIndex)
	{
		s_Gui->GuiDraw(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	}*/

	void Gui::OnRender()
	{
		s_Gui->GuiOnRender();
	}

	void Gui::OnRebuild()
	{
		s_Gui->GuiOnRebuild();
	}
}