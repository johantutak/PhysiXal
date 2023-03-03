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
		PX_PROFILE_FUNCTION();

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = GuiVulkan::GetGuiCommandPool();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(VulkanDevice::GetVulkanDevice(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void GuiEndSingleTimeCommands(VkCommandBuffer commandBuffer)
	{
		PX_PROFILE_FUNCTION();

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(VulkanDevice::GetVulkanGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(VulkanDevice::GetVulkanGraphicsQueue());

		vkFreeCommandBuffers(VulkanDevice::GetVulkanDevice(), GuiVulkan::GetGuiCommandPool(), 1, &commandBuffer);
	}
}