#pragma once

namespace PhysiXal {

	// Utility (Dear ImGUI) functions
	
	VkCommandBuffer GuiBeginSingleTimeCommands();
	void GuiEndSingleTimeCommands(VkCommandBuffer commandBuffer);
}