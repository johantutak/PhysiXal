#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

	class SystemStatistics
	{
	public:
		// Statistics
		void PrintStats();
	private:
		inline static VkPhysicalDeviceProperties s_Properties;
	};
}