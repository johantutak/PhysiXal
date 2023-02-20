#include "px_pch.h"
#include "core/application.h"

#include "core/input/input.h"
#include "core/window.h"

#include "renderer/renderer.h"

#include "gui/gui.h"

#include <GLFW/glfw3.h>

#include "utilities/system_statistics.h"

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Application
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
	{

		PX_PROFILE_FUNCTION();

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
		Renderer::Init();

		// Get system statistics 
		Utilities::SystemStatistics* m_Stats = nullptr;
		m_Stats->PrintStats();

		// #### TODO ####
		// Add func to execute command queue to compile all shaders

		// Initialize the GUI
		Gui::Init();
	}

	Application::~Application()
	{
		PX_PROFILE_FUNCTION();

		// Shutdown the GUI
		Gui::Shutdown();

		// Shutdown the renderer
		Renderer::Shutdown();


		// Destroy the window
		//Window::~Window(); It's only for symbolic purposes since the window is destroyed within it's own class.

		// Shutdown the application
		// #### TO DO #### Make it shutdown in he correct order.
		PX_WARN("...Shutting down Application (client).");
		PX_CORE_WARN("...Shutting down Log (core).");
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(PX_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(PX_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{ 
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			PX_PROFILE_SCOPE("Run Loop");

			float time = GetTime();
			m_FrameTime = time - m_LastFrameTime;
			m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					PX_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(m_FrameTime);
				}

				// Draw GUI frame here
				Gui::Begin();
				Gui::OnRender();
				Gui::End();

				// Draw frame here
				Renderer::DrawFrame();
			}

			m_Window->OnUpdate();

			Renderer::WaitAndIdle();
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

	float Application::GetTime() const
	{
		return (float)glfwGetTime();
	}
}