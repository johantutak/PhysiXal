#pragma once


#include "core/core.h"

#include "platform/win/win_window.h"
#include "core/layer_stack.h"
#include "events/event.h"
#include "events/app_event.h"

namespace PhysiXal {

	// From Hazel & Little Vulkan Engine

	class Application
	{
	public:
		Application();
		virtual	~Application();
		static constexpr int WIDTH = 1280;
		static constexpr int HEIGHT = 720;

		void OnEvent(Event& e);

		void PushLayer(CoreLayer* layer);
		void PushOverlay(CoreLayer* layer);

		void Run();
	private:
		LayerStack	m_LayerStack;
	private:
		WinWindow WinWindow{ WIDTH, HEIGHT, "PhysiXal Engine" };
	};

	// To be defined in CLIENT
	Application* CreateApplication();
} 