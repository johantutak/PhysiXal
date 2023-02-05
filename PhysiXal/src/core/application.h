#pragma once

#include "core/core.h"

#include "core/window.h"
#include "core/layer_stack.h"
#include "events/event.h"
#include "events/app_event.h"

#include "core/timestep.h"

#include "gui/gui.h"

int main(int argc, char** argv);

namespace PhysiXal {

	// From Hazel & Little Vulkan Engine

	struct ApplicationSpecification
	{
		std::string Name = "PhysiXal";
		uint32_t WindowWidth = 1600, WindowHeight = 900;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		virtual	~Application();

		void OnEvent(Event& e);

		void PushLayer(CoreLayer* layer);
		void PushOverlay(CoreLayer* layer);

		inline Window& GetWindow() { return *m_Window; }

		static Application& Get() { return *s_Instance; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;

		//GuiLayer* m_GuiLayer;

		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack	m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
		
		friend int ::main(int argc, char** argv);
	};

	// To be defined in CLIENT
	Application* CreateApplication();
} 