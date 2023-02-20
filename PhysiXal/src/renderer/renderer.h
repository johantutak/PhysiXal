#pragma once

#include <optional>

namespace PhysiXal {

	class Renderer
	{
	public:
		// Renderer
		static void Init();
		static void Shutdown();

		static void DrawFrame();

		static void WaitAndIdle();
	private:
	};
}