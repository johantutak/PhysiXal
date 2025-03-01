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

#include "log/log.h"

namespace PhysiXal {

    static Timestep* m_TimeStep = nullptr;

    // #### TEMPORARY #### have it false for now to get performance at start up.
    bool s_IsImGuiLoggingEnabled = false;

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

    void ImGuiWidgets::LogWindow()
    {
        ImGui::Begin("Log");

        if (ImGui::Button("Clear")) 
        {
            ClearLogMessages(); // Clear the log messages when the button is pressed
        }

        ImGui::SameLine();

        //bool isImGuiLoggingEnabled = ImGuiLogSink::GetIsImGuiLogEnabled();

        // Checkbox to toggle logging
        ImGui::Checkbox("Enable Logging", &s_IsImGuiLoggingEnabled);

        ImGui::Separator();

        // Begin a child window for log messages to make it scrollable
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);

        // Access messages from ImGuiLogSink
        auto& messages = dynamic_cast<ImGuiLogSink_mt&>(*Log::GetCoreLogger()->sinks().back()).GetMessages(); // Assuming the ImGui sink is the last one

        ImVec2 content_avail = ImGui::GetContentRegionAvail(); // For auto-scroll calculation

        // Here, we use zero to automatically wrap at the edge of the window.
        ImGui::PushTextWrapPos(0.0f);

        if (ImGui::BeginTable("LogMessages", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) // 1 column
        {
            ImGui::TableSetupColumn("Messages");
            ImGui::TableHeadersRow();

            bool alternateColor = false;

            for (const auto& msg : messages)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn(); // Even though it's a single column, you still need to call this

                // Alternate row color between black and gray
                ImVec4 bgColor = alternateColor ? ImVec4(0.3f, 0.3f, 0.3f, 1.0f) : ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // Gray and Black
                ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::ColorConvertFloat4ToU32(bgColor));

                // Set color based on the log level
                switch (msg.Level)
                {
                case spdlog::level::trace:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); // Grey
                    break;
                case spdlog::level::info:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 0.2f, 1.0f)); // Green
                    break;
                case spdlog::level::warn:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.2f, 1.0f)); // Yellow
                    break;
                case spdlog::level::err:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); // Red
                    break;
                case spdlog::level::critical:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f)); // Bright Red
                    break;
                default:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // White
                    break;
                }

                // Render the log message
                ImGui::TextUnformatted(msg.Message.c_str());

                // Don't forget to pop the color style
                ImGui::PopStyleColor();

                alternateColor = !alternateColor; // Toggle the alternate color for the next row
            }

            ImGui::EndTable();
        }

        // Restore the default text wrap position.
        ImGui::PopTextWrapPos();

        // Auto-scroll
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) 
        {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild(); // End the child window that contains the scrolling region

        ImGui::End();
    }

    void ImGuiWidgets::ClearLogMessages()
    {
        auto& sink = dynamic_cast<ImGuiLogSink_mt&>(*Log::GetCoreLogger()->sinks().back());
        sink.Clear();
    }
}