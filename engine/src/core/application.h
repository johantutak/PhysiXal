#pragma once

#include "core/core.h"

#include "core/window.h"
#include "core/layer_stack.h"
#include "events/event.h"
#include "events/app_event.h"

#include "core/timestep.h"

#include "gui/gui.h"

#include "scene/camera.h"

int main(int argc, char** argv);

namespace PhysiXal {

	struct ApplicationSpecification
	{
		std::string Name = "PhysiXal";
		U32 WindowWidth = 1600, WindowHeight = 900;
	};

	class Application
	{
	public:
		// Application
		Application(const ApplicationSpecification& specification);
		virtual	~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		Camera* GetCamera() const { return m_Camera; }

		static Application& Get() { return *s_Instance; }

		Timestep GetTimeStep() const { return m_TimeStep; }
		float GetTime() const; // TODO: This should be in "Platform / own header"
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;
		Camera* m_Camera;

		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		
		float m_LastFrameTime = 0.0f;
		Timestep m_FrameTime;
		Timestep m_TimeStep;
	private:
		static Application* s_Instance;
		
		friend int ::main(int argc, char** argv);
	};

	// To be defined in CLIENT
	Application* CreateApplication();
} 