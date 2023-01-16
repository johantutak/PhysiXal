#pragma once

#include <vulkan/vulkan.h>

#include "platform/win/win_window.h"

#include <optional>

namespace PhysiXal {

	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

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
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

		static VkPhysicalDevice GetVulkanPhysicalDevice() { return s_PhysicalDevice; }

		// Logical Device
		void CreateLogicalDevice();
		void DestroyDevice();
		
		static VkDevice GetVulkanDevice() { return s_LogicalDevice; }

		static VkQueue GetVulkanGraphicsQueue() { return s_GraphicsQueue; }
		static VkQueue GetVulkanPresentQueue() { return s_PresentQueue; }

		// Surface
		void CreateSurface();
		void DestroySurface();

		static VkSurfaceKHR GetVulkanSurface() { return s_Surface; }

		// Command pool
		void CreateCommandPool();

		static VkCommandPool GetVulkanCommandPool() { return s_CommandPool; }
	private:
		inline static VkPhysicalDevice s_PhysicalDevice = VK_NULL_HANDLE;
		
		inline static VkDevice s_LogicalDevice;

		inline static VkQueue s_GraphicsQueue;
		inline static VkQueue s_PresentQueue;

		inline static VkSurfaceKHR s_Surface;

		inline static VkCommandPool s_CommandPool;
	};
}