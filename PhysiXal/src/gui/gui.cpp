#include "px_pch.h"
#include "gui.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include "core/application.h"

#include "api/vulkan/vk_utilities.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

	void Gui::GuiInit()
	{
		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		auto vkInstance = VulkanContext::GetVulkanInstance();
		VkPhysicalDevice vkPhysicalDevice = VulkanDevice::GetVulkanPhysicalDevice();
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
		VkSampleCountFlagBits vkMsaaSamples = VulkanDevice::GetVulkanMsaa();
		VkCommandPool vkCommandPool = VulkanCommandBuffer::GetVulkanCommandPool();
		std::vector<VkCommandBuffer> vkCommandBuffer = VulkanCommandBuffer::GetVulkanCommandBuffers();
		VkQueue vkPresentQueue = VulkanDevice::GetVulkanPresentQueue();
		VkRenderPass vkRenderPass = VulkanRenderPass::GetVulkanRenderPass();
		std::vector<VkImageView> vkSwapChainImages = VulkanSwapChain::GetVulkanImageViews();
		VkDescriptorPool vkDescriptorPool = VulkanUniformBuffer::GetVulkanDescriptorPool();

		QueueFamilyIndices Indices = VulkanDevice::FindQueueFamilies(vkPhysicalDevice);
		
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear imGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		
		// #### TO DO #### implement viewport support
		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		/*ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}*/

		// Setup platform/renderer bindings
		ImGui_ImplGlfw_InitForVulkan(window, true);

		// This initializes imgui for Vulkan
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = vkInstance;
		init_info.PhysicalDevice = vkPhysicalDevice;
		init_info.Device = vkDevice;
		init_info.QueueFamily = Indices.m_GraphicsFamily.value();
		init_info.Queue = vkPresentQueue;
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = vkDescriptorPool;
		init_info.Allocator = nullptr;
		init_info.MinImageCount = 2;
		init_info.ImageCount = MAX_FRAMES_IN_FLIGHT;
		init_info.MSAASamples = vkMsaaSamples;
		init_info.CheckVkResultFn = NULL;
		ImGui_ImplVulkan_Init(&init_info, vkRenderPass);

		// Upload Fonts
		// Use any command queue
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();
		ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
		EndSingleTimeCommands(commandBuffer);

		vkDeviceWaitIdle(vkDevice);
		ImGui_ImplVulkan_DestroyFontUploadObjects();
	}

	void Gui::GuiShutdown()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Gui::GuiBegin()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Gui::GuiEnd(VkCommandBuffer commandBuffer)
	{	
		ImGui::Render();

		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

		/*ImGuiIO& io = ImGui::GetIO(); (void)io;
		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}*/
	}

	void Gui::GuiOnRender()
	{
		ImGui::ShowDemoWindow();
	}
}