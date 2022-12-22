#pragma once

#ifdef PX_PLATFORM_WINDOWS
	#include <vulkan/vulkan.h>
#endif

#include <optional>

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> m_GraphicsFamily;

		bool IsComplete()
		{
			return m_GraphicsFamily.has_value();
		}
	};

	class VulkanDevice
	{
	public:
		// Physical Device
		static void PickPhysicalDevice();

		static bool IsDeviceSuitable(VkPhysicalDevice device);
		static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

		//  Logical Device
		static void CreateLogicalDevice();
		static void DestroyDevice();

	private:
		inline static VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		inline static VkDevice m_LogicalDevice;

		inline static VkQueue m_GraphicsQueue;
	};
#endif
}