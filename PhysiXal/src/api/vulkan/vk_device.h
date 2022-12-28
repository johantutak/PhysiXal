#pragma once

#ifdef PX_PLATFORM_WINDOWS
	#include <vulkan/vulkan.h>
#endif

#include "platform/win/win_window.h"

#include <optional>

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	struct QueueFamilyIndices {
		std::optional<uint32_t> m_GraphicsFamily;
		std::optional<uint32_t> m_PresentFamily;

		bool IsComplete() {
			return m_GraphicsFamily.has_value() && m_PresentFamily.has_value();
		}
	};

	class VulkanDevice
	{
	public:
		// Physical Device
		void PickPhysicalDevice();
		bool IsDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

		//  Logical Device
		void CreateLogicalDevice();
		void DestroyDevice();

		// Surface
		void CreateSurface();
		void DestroySurface();
	private:
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		
		VkDevice m_LogicalDevice;

		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;

		VkSurfaceKHR m_Surface;
	};
#endif
}