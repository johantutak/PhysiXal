#include "px_pch.h"
#include "api/imgui/imgui_utilities.h"

#include "api/imgui/imgui_initializers.h"

#include "api/vulkan/vk_initializers.h"

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Utility (Dear ImGUI) functions
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	VkCommandBuffer GuiBeginSingleTimeCommands()
	{
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
		VkCommandPool vkGuiCommandPool = GuiVulkan::GetGuiCommandPool();

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = vkGuiCommandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(vkDevice, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void GuiEndSingleTimeCommands(VkCommandBuffer commandBuffer)
	{
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
		VkQueue vkGraphicsQueue = VulkanDevice::GetVulkanGraphicsQueue();
		VkCommandPool vkGuiCommandPool = GuiVulkan::GetGuiCommandPool();

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(vkGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(vkGraphicsQueue);

		vkFreeCommandBuffers(vkDevice, vkGuiCommandPool, 1, &commandBuffer);
	}
}