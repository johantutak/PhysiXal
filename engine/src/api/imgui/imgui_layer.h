#pragma once

#include "events/app_event.h"
#include "events/key_event.h"
#include "events/mouse_event.h"

#include <vulkan/vulkan.h>

namespace PhysiXal {

	class GuiLayer
	{
	public:
		// Dear ImGUI
		void GuiInit();
		void GuiShutdown();
		
		void GuiBegin();
		void GuiEnd();
		
		void GuiDraw(VkCommandBuffer commandBuffer, U32 imageIndex);

		void GuiOnRender();

		void GuiOnRebuild();
	private:

	};
}