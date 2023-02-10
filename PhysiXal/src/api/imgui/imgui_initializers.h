#pragma once

#include "api/imgui/imgui_layer.h"
#include "api/imgui/imgui_vk.h"

namespace PhysiXal {

	// Dear ImGUI pointers (initializers)
	static GuiLayer* m_Gui = nullptr;
	static GuiVulkan* m_GuiVulkan = nullptr;
}