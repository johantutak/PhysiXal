#pragma once

#include "events/app_event.h"
#include "events/key_event.h"
#include "events/mouse_event.h"

#include <vulkan/vulkan.h>

namespace PhysiXal {

	class Gui
	{
	public:
		void GuiInit();
		void GuiShutdown();
		
		void GuiBegin();
		void GuiEnd(VkCommandBuffer commandBuffer);

		void GuiOnRender();
	private:

	};
}