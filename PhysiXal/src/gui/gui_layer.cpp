#include "px_pch.h"
#include "gui/gui_Layer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include "core/application.h"

#include "api/vulkan/vk_context.h"
#include "api/vulkan/vk_device.h"
#include "api/vulkan/vk_render_pass.h"
#include "api/vulkan/vk_swap_chain.h"
#include "api/vulkan/vk_command_buffer.h"

// #### TEMPORARY ####
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

	GuiLayer::GuiLayer()
		: CoreLayer("GuiLayer")
	{
	}

	GuiLayer::~GuiLayer()
	{
	}

	void GuiLayer::OnAttach()
	{
		/*auto vkInstance = VulkanContext::GetVulkanInstance();
		VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
		VkPhysicalDevice vkPhysicalDevice = VulkanDevice::GetVulkanPhysicalDevice();
		VkQueue vkPresentQueue = VulkanDevice::GetVulkanPresentQueue();
		VkRenderPass vkRenderPass = VulkanRenderPass::GetVulkanRenderPass();
		std::vector<VkImageView> vkSwapChainImages = VulkanSwapChain::GetVulkanImageViews();

		QueueFamilyIndices Indices = VulkanDevice::FindQueueFamilies(vkPhysicalDevice);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoDecoration = false;
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

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

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

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
		init_info.DescriptorPool = m_GuiPool;
		init_info.Allocator = nullptr;
		init_info.MinImageCount = 2;
		init_info.ImageCount = static_cast<uint32_t>(vkSwapChainImages.size());;
		init_info.CheckVkResultFn = NULL;

		ImGui_ImplVulkan_Init(&init_info, m_GuiRenderPass);

		// Upload Fonts
		{
			// Use any command queue
			VkCommandBuffer GuicommandBuffer = BeginSingleTimeCommands;
			ImGui_ImplVulkan_CreateFontsTexture(GuicommandBuffer);
			EndSingleTimeCommands(GuicommandBuffer);

			VkResult(vkDeviceWaitIdle(vkDevice));
			ImGui_ImplVulkan_DestroyFontUploadObjects();
		}*/
	}

	void GuiLayer::OnDetach()
	{
		/*VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

		// Destroy the imgui created structures
		vkDestroyDescriptorPool(vkDevice, m_GuiPool, nullptr);
		VkResult(vkDeviceWaitIdle(vkDevice));

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();*/
	}

	/* void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}*/

	void GuiLayer::Begin()
	{
		/*ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();*/
	}

	void GuiLayer::End()
	{
		/*ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();

		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			//GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			//glfwMakeContextCurrent(backup_current_context);
		}*/
	}

	// #### TEMPORARY ####
	void GuiLayer::GuiDescriptorPool()
	{
		/*VkDevice vkDevice = VulkanDevice::GetVulkanDevice();

		// Create Descriptor Pool
		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000;
		pool_info.poolSizeCount = std::size(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;
		VkResult(vkCreateDescriptorPool(vkDevice, &pool_info, nullptr, &m_GuiPool));*/
	}

	void GuiLayer::GuiRenderPass()
	{
		/*VkDevice vkDevice = VulkanDevice::GetVulkanDevice();
		VkFormat vkSwapChainImageFormat = VulkanSwapChain::GetVulkanImageFormat();

		VkAttachmentDescription attachment = {};
		attachment.format = vkSwapChainImageFormat;
		attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference color_attachment = {};
		color_attachment.attachment = 0;
		color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &color_attachment;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;  // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.attachmentCount = 1;
		info.pAttachments = &attachment;
		info.subpassCount = 1;
		info.pSubpasses = &subpass;
		info.dependencyCount = 1;
		info.pDependencies = &dependency;
		
		if (vkCreateRenderPass(vkDevice, &info, nullptr, &m_GuiRenderPass) != VK_SUCCESS)
		{
			PX_CORE_ERROR("Failed to create GUI's render pass!");
		}*/
	}
}