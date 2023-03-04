#pragma once

#include <optional>

namespace PhysiXal {

	class Renderer
	{
	public:
		// Renderer
		static void Init();
		static void Shutdown();

		static void BeginFrame();
		static void EndFrame();

		static void WaitAndIdle();
	private:
	};
}