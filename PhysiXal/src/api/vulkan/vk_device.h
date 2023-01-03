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

		// Logical Device
		void CreateLogicalDevice();
		void DestroyDevice();
		
		static VkDevice GetVulkanDevice() { return s_LogicalDevice; }

		static VkQueue GetVulkanGraphicsQueue() { return s_GraphicsQueue; }
		static VkQueue GetVulkanPresentQueue() { return s_PresentQueue; }

		// Surface
		void CreateSurface();
		void DestroySurface();

		// Swap chain
		void CreateSwapChain();
		void DestroySwapChain();
		
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		static VkSwapchainKHR GetVulkanSwapChain() { return s_SwapChain; }

		static VkFormat GetVulkanImageFormat() { return s_SwapChainImageFormat; }
		static VkExtent2D GetVulkanSwapChainExtent() { return s_SwapChainExtent; }
		
		// Image views
		void CreateImageViews();
		void DestroyImageViews();

		static std::vector<VkImageView> GetVulkanImageViews() { return s_SwapChainImageViews; }

		// Command Pool
		void CreateCommandPool();
		void DestroyCommandPool();

		static VkCommandPool GetVulkanCommandPool() { return s_CommandPool; }
	private:
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		
		inline static VkDevice s_LogicalDevice;

		inline static VkQueue s_GraphicsQueue;
		inline static VkQueue s_PresentQueue;

		VkSurfaceKHR m_Surface;
		
		inline static VkSwapchainKHR s_SwapChain;
		std::vector<VkImage> m_SwapChainImages;
		inline static VkFormat s_SwapChainImageFormat;
		inline static VkExtent2D s_SwapChainExtent;

		inline static std::vector<VkImageView> s_SwapChainImageViews;

		inline static VkCommandPool s_CommandPool;
	};
#endif
}