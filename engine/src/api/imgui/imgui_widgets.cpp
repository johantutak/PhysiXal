#include "px_pch.h"
#include "api/imgui/imgui_widgets.h"

#include <imgui.h>
#include "api/imgui/imgui_initializers.h"

#include "core/application.h"

#include "api/vulkan/vk_initializers.h"
#include "api/vulkan/vk_utilities.h"

#include "core/timestep.h"

#include "utilities/cpu_id.h"

#include "core/input/input.h"

#include "platform/win/win_utilities.h"
#include "utilities/string_utilities.h"

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

		U32 vk_major = VK_VERSION_MAJOR(properties.apiVersion);
        U32 vk_minor = VK_VERSION_MINOR(properties.apiVersion);
        U32 vk_patch = VK_VERSION_PATCH(properties.apiVersion);

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
		ImGui::Text("FRAME TIME: %.4f: ms/frame - %.4f: sec/frame - (%.0f FPS) \n", app.GetTimeStep().GetMilliseconds(), 
			app.GetTimeStep().GetSeconds(), app.GetTimeStep().GetFramesPerSecond());
		
		ImGui::Separator();

		ImGui::Text("MOUSE POSITION: X:(%.0f) Y:(%.0f)\n", Input::GetMouseX(), Input::GetMouseY());

		ImGui::End();
	}

    void ImGuiWidgets::VertexData()
    {
        ImGui::Begin("Vertex Data");

        ImGui::Columns(4, "vertex_columns", true); // Create four columns

        // Set column headers
        ImGui::Text("Index");
        ImGui::NextColumn();
        ImGui::Text("Position");
        ImGui::NextColumn();
        ImGui::Text("Texture Coordinates");
        ImGui::NextColumn();
        ImGui::Text("Color");
        ImGui::NextColumn();

        // Add a separator below column headers
        ImGui::Separator();

        ImGuiListClipper clipper;
        clipper.Begin(m_Mesh->GetVulkanMesh().vertices.size());

        while (clipper.Step())
        {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            {
                ImGui::Text("%d", i + 1);
                ImGui::NextColumn();

                ImGui::Text("%.3f, %.3f, %.3f", m_Mesh->GetVulkanMesh().vertices[i].pos.x, m_Mesh->GetVulkanMesh().vertices[i].pos.y, m_Mesh->GetVulkanMesh().vertices[i].pos.z);
                ImGui::NextColumn();

                ImGui::Text("%.3f, %.3f", m_Mesh->GetVulkanMesh().vertices[i].texCoord.x, m_Mesh->GetVulkanMesh().vertices[i].texCoord.y);
                ImGui::NextColumn();

                ImGui::Text("%.3f, %.3f, %.3f", m_Mesh->GetVulkanMesh().vertices[i].color.x, m_Mesh->GetVulkanMesh().vertices[i].color.y, m_Mesh->GetVulkanMesh().vertices[i].color.z);
                ImGui::NextColumn();
            }
        }

        ImGui::Columns(1); // Reset to a single column layout

        ImGui::End();
    }
}