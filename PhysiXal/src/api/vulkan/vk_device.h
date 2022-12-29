#pragma once

#ifdef PX_PLATFORM_WINDOWS
	#include <vulkan/vulkan.h>
#endif

#include "platform/win/win_window.h"

#include <optional>

namespace PhysiXal {

#ifdef PX_PLATFORM_WINDOWS

	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	struct QueueFamilyIndices {
		std::optional<uint32_t> m_GraphicsFamily;
		std::optional<uint32_t> m_PresentFamily;

		bool IsComplete() {
			return m_GraphicsFamily.has_value() && m_PresentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanDevice
	{
	public:
		// Physical Device
		void PickPhysicalDevice();
		bool IsDeviceSuitable(VkPhysicalDevice device);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

		//  Logical Device
		void CreateLogicalDevice();
		void DestroyDevice();

		// Surface
		void CreateSurface();
		void DestroySurface();

		// SwapChain
		void CreateSwapChain();
		void DestroySwapChain();
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	private:
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		
		VkDevice m_LogicalDevice;

		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;

		VkSurfaceKHR m_Surface;
		
		VkSwapchainKHR m_SwapChain;
		std::vector<VkImage> m_SwapChainImages;
		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;
	};
#endif
}