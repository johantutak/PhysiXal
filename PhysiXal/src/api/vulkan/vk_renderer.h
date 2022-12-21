#pragma once

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	class VulkanRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginFrame();
	private:
	};
#endif
}