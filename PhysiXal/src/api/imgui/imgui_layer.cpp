#include "px_pch.h"
#include "api/imgui/imgui_layer.h"

#include <imgui.h>
#include "api/imgui//backends/gui_glfw_backend.h"
#include "api/imgui//backends/gui_vk_backend.h"
#include "api/imgui/imgui_initializers.h"
#include "api/imgui/imgui_utilities.h"

#include "core/application.h"

#include "api/vulkan/vk_initializers.h"

#include "core/timestep.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

	static Timestep* m_TimeStep = nullptr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Dear ImGUI
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void GuiLayer::GuiInit()
	{
		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		auto vkInstance = VulkanContext::GetVulkanInstance();
		VkPhysicalDevice vkPhysicalDevice = VulkanDevice::GetVulkanPhysicalDevice();
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
		VkCommandPool vkCommandPool = VulkanCommandBuffer::GetVulkanCommandPool();
		std::vector<VkCommandBuffer> vkCommandBuffer = VulkanCommandBuffer::GetVulkanCommandBuffers();
		VkQueue vkPresentQueue = VulkanDevice::GetVulkanPresentQueue();
		std::vector<VkImageView> vkSwapChainImages = VulkanSwapChain::GetVulkanImageViews();
		
		PX_CORE_INFO("Initializing the GUI (Dear ImGUI)");

		QueueFamilyIndices Indices = VulkanDevice::FindQueueFamilies(vkPhysicalDevice);
		
		// #### Initialize Vulkan (Dear ImGUI) ####
		m_GuiVulkan->CreateGuiDescriptorPool();
		m_GuiVulkan->CreateGuiRenderPass();
		m_GuiVulkan->CreateGuiCommandPool();
		m_GuiVulkan->CreateGuiCommandBuffers();
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
		init_info.Instance = vkInstance;
		init_info.PhysicalDevice = vkPhysicalDevice;
		init_info.Device = vkDevice;
		init_info.QueueFamily = Indices.m_GraphicsFamily.value();
		init_info.Queue = vkPresentQueue;
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = m_GuiVulkan->GetGuiDescriptorPool();
		init_info.Allocator = nullptr;
		init_info.MinImageCount = MAX_FRAMES_IN_FLIGHT;
		init_info.ImageCount = static_cast<uint32_t>(vkSwapChainImages.size());
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.CheckVkResultFn = NULL;
		
		ImGui_ImplVulkan_Init(&init_info, m_GuiVulkan->GetGuiRenderPass());

		// #### Execute a gpu command to upload imgui font textures ####
		
		// Use any command queue
		VkCommandBuffer commandBuffer = GuiBeginSingleTimeCommands();
		ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
		GuiEndSingleTimeCommands(commandBuffer);

		vkDeviceWaitIdle(vkDevice);
		ImGui_ImplVulkan_DestroyFontUploadObjects();
	}

	void GuiLayer::GuiShutdown()
	{
		PX_CORE_WARN("...Shutting down the GUI (Dear ImGUI)");

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		m_GuiVulkan->DestroyGuiFramebuffers();
		m_GuiVulkan->DestroyGuiCommandBuffers();
		m_GuiVulkan->DestroyGuiCommandPool();
		m_GuiVulkan->DestroyGuiRenderPass();
		m_GuiVulkan->DestroyGuiDescriptorPool();
	}

	void GuiLayer::GuiBegin()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void GuiLayer::GuiEnd()
	{	
		ImGui::Render();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void GuiLayer::GuiDraw(VkCommandBuffer commandBuffer, uint32_t imageIndex)
	{
		VkExtent2D vkSwapChainExtent2D = VulkanSwapChain::GetVulkanSwapChainExtent();
		uint32_t vkCurrentFrame = VulkanRenderer::GetVulkanCurrentFrame();

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
		Application& app = Application::Get();

		static bool showDemoWindow = true;
		ImGui::ShowDemoWindow(&showDemoWindow);
		
		ImGui::Begin("FPS / Frame time");
		ImGui::Text("Frames per second:     %.0f: fps\n", app.GetTimeStep().GetFramesPerSecond());
		
		ImGui::Text("Frame time (ms):   %.4f: sec\n", app.GetTimeStep().GetSeconds());
		ImGui::Text("Frame time (sec): %.4f: ms\n", app.GetTimeStep().GetMilliseconds());
		ImGui::End();
	}

	void GuiLayer::GuiOnRebuild()
	{
		ImGui_ImplVulkan_SetMinImageCount(MAX_FRAMES_IN_FLIGHT);
		m_GuiVulkan->CreateGuiRenderPass();
		m_GuiVulkan->CreateGuiFramebuffers();
		m_GuiVulkan->CreateGuiCommandBuffers();
	}
}