#pragma once

#include <optional>

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	class VulkanSwapChain
	{
	public:
		// Swap Chain
		void InitSwapChain();
		void ShutdownSwapChain();
	private:
	};
#endif
}