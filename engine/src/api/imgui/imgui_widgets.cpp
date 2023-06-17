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

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "utilities/string_utilities.h"

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

	void ImGuiWidgets::AssetManager()
	{
        ImGui::Begin("Asset Manager");

        // Mesh selection
        if (ImGui::Button("Mesh")) 
        {
            if (FileManager::SelectFile(m_SelectedModelFile, TEXT("Mesh Files\0*.obj\0All Files\0*.*\0")))
            {
                // File selected successfully
                m_FileSelected = true;
            }
        }
        ImGui::Text("%s", m_SelectedModelFile.c_str());

        // Vertex shader selection
        if (ImGui::Button("Vertex Shader"))
        {
            if (FileManager::SelectFile(m_SelectedVertexShaderFile, TEXT("Vertex Shader Files\0*.spv\0All Files\0*.*\0")))
            {
                // File selected successfully
                m_FileSelected = true;
            }
        }
        ImGui::Text("%s", m_SelectedVertexShaderFile.c_str());

        // Fragment shader selection
        if (ImGui::Button("Fragment Shader"))
        {
            if (FileManager::SelectFile(m_SelectedFragmentShaderFile, TEXT("Fragment Shader Files\0*.spv\0All Files\0*.*\0")))
            {
                // File selected successfully
                m_FileSelected = true;
            }
        }
        ImGui::Text("%s", m_SelectedFragmentShaderFile.c_str());

        // Texture selection
        if (ImGui::Button("Texture"))
        {
            if (FileManager::SelectFile(m_SelectedTextureFile, TEXT("Image Files\0*.png;*.jpg;*.bmp\0All Files\0*.*\0")))
            {
                // File selected successfully
                m_FileSelected = true;
            }
        }
        ImGui::Text("%s", m_SelectedTextureFile.c_str());

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
        clipper.Begin(m_Model->GetVulkanVertices().size());

        while (clipper.Step())
        {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            {
                ImGui::Text("%d", i + 1);
                ImGui::NextColumn();

                ImGui::Text("%.3f, %.3f, %.3f", m_Model->GetVulkanVertices()[i].pos.x, m_Model->GetVulkanVertices()[i].pos.y, m_Model->GetVulkanVertices()[i].pos.z);
                ImGui::NextColumn();

                ImGui::Text("%.3f, %.3f", m_Model->GetVulkanVertices()[i].texCoord.x, m_Model->GetVulkanVertices()[i].texCoord.y);
                ImGui::NextColumn();

                ImGui::Text("%.3f, %.3f, %.3f", m_Model->GetVulkanVertices()[i].color.x, m_Model->GetVulkanVertices()[i].color.y, m_Model->GetVulkanVertices()[i].color.z);
                ImGui::NextColumn();
            }
        }

        ImGui::Columns(1); // Reset to a single column layout

        ImGui::End();
    }
}