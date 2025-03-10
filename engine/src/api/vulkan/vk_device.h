#pragma once

#include <vulkan/vulkan.h>

#include "platform/win/win_window.h"

#include <optional>

namespace PhysiXal {

	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	struct QueueFamilyIndices {
		std::optional<U32> m_GraphicsFamily;
		std::optional<U32> m_PresentFamily;

		bool IsComplete() {
			return m_GraphicsFamily.has_value() && m_PresentFamily.has_value();
		}
	};

	class VulkanDevice
	{
	public:
		// Physical Device
		void PickPhysicalDevice();
		
		int RateDeviceSuitability(VkPhysicalDevice device);
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

		// Multisampling (MSAA)
		void CreateColorResources();
		void DestroyColorResources();

		static VkSampleCountFlagBits GetVulkanMsaa() { return s_MsaaSamples; }
		static VkImageView GetVulkanColorImageView() { return s_ColorImageView; };
	private:
		inline static VkPhysicalDevice s_PhysicalDevice = VK_NULL_HANDLE;
		
		inline static VkDevice s_LogicalDevice;

		inline static VkQueue s_GraphicsQueue;
		inline static VkQueue s_PresentQueue;

		inline static VkSampleCountFlagBits s_MsaaSamples = VK_SAMPLE_COUNT_1_BIT;
		inline static VkImage s_ColorImage;
		inline static VkDeviceMemory s_ColorImageMemory;
		inline static VkImageView s_ColorImageView;
	};
}