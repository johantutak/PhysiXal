#pragma once

#include <optional>

namespace PhysiXal {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginFrame();

		static void WaitAndIdle();
	private:
	};
}