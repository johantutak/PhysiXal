#pragma once

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	class VulkanRenderer
	{
	public:
		void Init();
		void Shutdown();

		void BeginFrame();
	private:
	};
#endif
}