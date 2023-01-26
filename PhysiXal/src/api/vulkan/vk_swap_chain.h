#pragma once


#include <vulkan/vulkan.h>

#include "platform/win/win_window.h"

namespace PhysiXal {

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanSwapChain
	{
	public:
		// Swap chain
		void CreateSwapChain();
		void DestroySwapChain();

		static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
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
	private:
		inline static VkSwapchainKHR s_SwapChain;
		inline static std::vector<VkImage> s_SwapChainImages;
		inline static VkFormat s_SwapChainImageFormat;
		inline static VkExtent2D s_SwapChainExtent;

		inline static std::vector<VkImageView> s_SwapChainImageViews;
	};
}