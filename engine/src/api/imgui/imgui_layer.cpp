#include "px_pch.h"
#include "api/imgui/imgui_layer.h"

#include <imgui.h>
#include "api/imgui//backends/gui_glfw_backend.h"
#include "api/imgui//backends/gui_vk_backend.h"
#include "api/imgui/imgui_initializers.h"
#include "api/imgui/imgui_utilities.h"
#include "api/imgui/imgui_widgets.h"

#include "core/application.h"

#include "api/vulkan/vk_initializers.h"
#include "api/vulkan/vk_utilities.h"

#include "asset/asset_manager.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

	static ImGuiWidgets* m_Widgets = nullptr;
	static AssetManager* m_AssetManager = nullptr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Dear ImGUI
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void GuiLayer::GuiInit()
	{
		PX_PROFILE_FUNCTION();
		
		PX_CORE_INFO("Initializing the GUI (Dear ImGUI)");

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		QueueFamilyIndices Indices = VulkanDevice::FindQueueFamilies(VulkanDevice::GetVulkanPhysicalDevice());
		
		// #### Initialize Vulkan (Dear ImGUI) ####

		// Create Dear ImGUIs descriptor pool
		m_GuiVulkan->CreateGuiDescriptorPool();

		// Create Dear ImGUI render pass
		m_GuiVulkan->CreateGuiRenderPass();

		// Create Dear ImGUI command pool
		m_GuiVulkan->CreateGuiCommandPool();

		// Create Dear ImGUI command buffers
		m_GuiVulkan->CreateGuiCommandBuffers();

		// Create Dear ImGUI framebuffers
		m_GuiVulkan->CreateGuiFramebuffers();

		// #### Initialize ImGUI library ####
		// This initializes the core structures of Dear ImGui (context)
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Enables IO
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear imGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		
		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// #### Setup platform/renderer bindings ####
		
		// This initializes Dear ImGui for GLFW
		ImGui_ImplGlfw_InitForVulkan(window, true);

		// This initializes imgui for Vulkan
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = VulkanContext::GetVulkanInstance();
		init_info.PhysicalDevice = VulkanDevice::GetVulkanPhysicalDevice();
		init_info.Device = VulkanDevice::GetVulkanDevice();
		init_info.QueueFamily = Indices.m_GraphicsFamily.value();
		init_info.Queue = VulkanDevice::GetVulkanPresentQueue();
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = m_GuiVulkan->GetGuiDescriptorPool();
		init_info.Allocator = nullptr;
		init_info.MinImageCount = c_MaxImageCount;
		init_info.ImageCount = static_cast<U32>(VulkanSwapChain::GetVulkanImageViews().size());
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.CheckVkResultFn = NULL;
		
		ImGui_ImplVulkan_Init(&init_info, m_GuiVulkan->GetGuiRenderPass());

		// #### Execute a gpu command to upload imgui font textures ####
		
		// Use any command queue
		VkCommandBuffer commandBuffer = GuiBeginSingleTimeCommands();
		ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
		GuiEndSingleTimeCommands(commandBuffer);

		vkDeviceWaitIdle(VulkanDevice::GetVulkanDevice());
		ImGui_ImplVulkan_DestroyFontUploadObjects();
	}

	void GuiLayer::GuiShutdown()
	{
		PX_PROFILE_FUNCTION();

		PX_CORE_WARN("...Shutting down the GUI (Dear ImGUI)");

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		// Destruction of Dear ImGUI framebuffers
		m_GuiVulkan->DestroyGuiFramebuffers();

		// Destruction of Dear ImGUI command buffers
		m_GuiVulkan->DestroyGuiCommandBuffers();

		// Destruction of Dear ImGUI command pool
		m_GuiVulkan->DestroyGuiCommandPool();

		// Destruction of Dear ImGUI render pass
		m_GuiVulkan->DestroyGuiRenderPass();

		// Destruction of Dear ImGUI descriptor pool
		m_GuiVulkan->DestroyGuiDescriptorPool();
	}

	void GuiLayer::GuiBegin()
	{
		PX_PROFILE_FUNCTION();

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void GuiLayer::GuiEnd()
	{	
		PX_PROFILE_FUNCTION();

		ImGui::Render();

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void GuiLayer::GuiDraw(VkCommandBuffer commandBuffer, U32 imageIndex)
	{
		PX_PROFILE_FUNCTION();

		VkExtent2D vkSwapChainExtent2D = VulkanSwapChain::GetVulkanSwapChainExtent();
		U32 vkCurrentFrame = VulkanRenderer::GetVulkanCurrentFrame();

		std::vector<VkFramebuffer> vkGuiFramebuffers = GuiVulkan::GetGuiFramebuffers();
		VkRenderPass vkGuiRenderPass = GuiVulkan::GetGuiRenderPass();

		// vkResetCommandPool(vkDevice, s_GuiCommandPool, 0);
		VkCommandBufferBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		vkBeginCommandBuffer(commandBuffer, &info);

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = vkGuiRenderPass;
		renderPassInfo.framebuffer = vkGuiFramebuffers[imageIndex];
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = vkSwapChainExtent2D;
		VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;
		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		// Record dear imgui primitives into command buffer
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

		vkCmdEndRenderPass(commandBuffer);
		vkEndCommandBuffer(commandBuffer);
	}

	void GuiLayer::GuiOnRender()
	{
		PX_PROFILE_FUNCTION();
		
		// Issues Dear ImGui command for performance stats widget
		m_Widgets->PerformanceStats();
		m_Widgets->AssetManager(); // #### TEMPORARY ####
		m_AssetManager->Manager();
		//m_Widgets->VertexData(); // #### TODO #### Fix performance and make it only display if 3D object is selected
	}

	void GuiLayer::GuiOnRebuild()
	{
		PX_PROFILE_FUNCTION();

		ImGui_ImplVulkan_SetMinImageCount(c_MaxImageCount);

		// On framebuffer rebuild we recreate Dear ImGUI render pass
		m_GuiVulkan->CreateGuiRenderPass();

		// On framebuffer rebuild we recreate Dear ImGUI framebuffers
		m_GuiVulkan->CreateGuiFramebuffers();

		// On framebuffer rebuild we recreate Dear ImGUIscommand buffers
		m_GuiVulkan->CreateGuiCommandBuffers();
	}
}