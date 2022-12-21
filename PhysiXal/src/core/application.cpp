#include "px_pch.h"
#include "core/application.h"

#include "core/input/input.h"
#include "core/window.h"

#include "api/vulkan/vk_renderer.h"

#include <GLFW/glfw3.h>

namespace PhysiXal {

	// From Hazel & Little Vulkan Engine

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
	{
		PX_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		// Initialize the window
		WindowSpecification windowSpec;
		windowSpec.Title = specification.Name;
		windowSpec.Width = specification.WindowWidth;
		windowSpec.Height = specification.WindowHeight;
		m_Window = std::unique_ptr<Window>(Window::Create(windowSpec));
		m_Window->Init();
		m_Window->SetEventCallback(PX_BIND_EVENT_FN(Application::OnEvent));

		// Initialize the renderer
#ifdef PX_PLATFORM_WINDOWS
		VulkanRenderer::Init();
#endif

		// #### TODO ####
		// Add func to execute command queue to compile all shaders
	}

	Application::~Application()
	{
		// Shutdown the renderer
#ifdef PX_PLATFORM_WINDOWS
		VulkanRenderer::Shutdown();
#endif

		// Destroy the window
		//Window::~Window(); It's only for symbolic purposes since the window is destroyed within it's own class.

		// Shutdown the application
		// #### TO DO #### Make it Shutdown in he correct order.
		PX_WARN("...Shutting down Application (client).");
		PX_CORE_WARN("...Shutting down Log (core).");
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(PX_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(CoreLayer* layer)
	{ 
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(CoreLayer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			// #### TEMPORARY #### 
			// Add class that is general rather than api specific like exmaple -> (Platform::GetTime();)
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					for (CoreLayer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				//Draw frame here
#ifdef PX_PLATFORM_WINDOWS
				VulkanRenderer::BeginFrame();
#endif

			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;

		return false;
	}
}