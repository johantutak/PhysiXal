#include "px_pch.h"
#include "api/imgui/imgui_widgets.h"

#include <imgui.h>
#include "api/imgui/imgui_initializers.h"

#include "core/application.h"

#include "api/vulkan/vk_initializers.h"
#include "api/vulkan/vk_utilities.h"

#include "core/timestep.h"

#include "utilities/cpu_id.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

	static Timestep* m_TimeStep = nullptr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Dear ImGUI widgets
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ImGuiWidgets::PerformanceStats()
	{
		PX_PROFILE_FUNCTION();

        Application& app = Application::Get();

		CPUInfo cinfo;

		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof(statex);
		GlobalMemoryStatusEx(&statex);

		VkPhysicalDevice vkPhysicalDevice = VulkanDevice::GetVulkanPhysicalDevice();
		VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(vkPhysicalDevice, &properties);

		uint32_t vk_major = VK_VERSION_MAJOR(properties.apiVersion);
        uint32_t vk_minor = VK_VERSION_MINOR(properties.apiVersion);
        uint32_t vk_patch = VK_VERSION_PATCH(properties.apiVersion);

		ImGui::Begin("Performance (DEVICE)");

		ImGui::Text("VENDOR: %s\n", cinfo.vendor().c_str());
		ImGui::Text("DEVICE (CPU): %s\n", cinfo.model().c_str());
		ImGui::Text("CORES/THREADS: %u\n", cinfo.cores());

		ImGui::Separator();

		ImGui::Text("\n");
		ImGui::Text("MEMORY (RAM): %.2f Gigabyte\n", (float)statex.ullTotalPhys / (1024 * 1024 * 1024));

		ImGui::Separator();

		ImGui::Text("\n");
		ImGui::Text("VENDOR: %s\n", VulkanVendorIDToString(properties.vendorID));
		ImGui::Text("DEVICE (GPU): %s\n", properties.deviceName);
		ImGui::Text("TYPE: %s\n", VulkanDeviceTypeToString(properties.deviceType));
		ImGui::Text("API VERSION: %u.%u.%u\n", vk_major, vk_minor, vk_patch);

		ImGui::Separator();

		ImGui::Text("\n");
		ImGui::Text("FRAME TIME: %.4f: ms/frame - %.4f: sec/frame - (%.0f FPS) \n", app.GetTimeStep().GetMilliseconds(), app.GetTimeStep().GetSeconds(), app.GetTimeStep().GetFramesPerSecond());
		
		ImGui::End();
	}
}