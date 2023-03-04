#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

	class Gui
	{
	public:
		// GUI
		static void Init();
		static void Shutdown();

		static void Begin();
		static void End();

		// #### TO DO #### Replace GuiLayer this function 
		static void Draw();

		static void OnRender();

		static void OnRebuild();
	};
}