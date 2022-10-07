#pragma once

#include "core/core.h"

#include "core/window.h"
#include "core/layer_stack.h"
#include "events/event.h"
#include "events/app_event.h"

int main(int argc, char** argv);

namespace PhysiXal {

	// From Hazel & Little Vulkan Engine

	class Application
	{
	public:
		Application();
		virtual	~Application();
		
		void OnEvent(Event& e);

		void PushLayer(CoreLayer* layer);
		void PushOverlay(CoreLayer* layer);

		Window& GetWindow() { return *m_Window; }

		static Application& Get() { return *s_Instance; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack	m_LayerStack;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);

	};

	// To be defined in CLIENT
	Application* CreateApplication();
} 