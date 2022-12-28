#pragma once

#include <optional>

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginFrame();
	private:
	};
#endif
}