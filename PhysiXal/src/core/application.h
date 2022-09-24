#pragma once

#include "main/core.h"

#include "platform/win/win_window.h"


namespace PhysiXal {

	class Application
	{
	public:
		Application();
		virtual	~Application();
		static constexpr int WIDTH = 1280;
		static constexpr int HEIGHT = 720;

		void Run();
	private:
		WinWindow WinWindow{ WIDTH, HEIGHT, "PhysiXal Engine" };
	};

	// To be defined in CLIENT
	Application* CreateApplication();
} 