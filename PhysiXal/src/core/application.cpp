#include "pch/px_pch.h"

#include "application.h"

#include "platform/win/win_window.h"

#include "main/log.h"

namespace PhysiXal {

	Application::Application()
	{	
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (!WinWindow.shouldClose()) {
			glfwPollEvents();
		}
	}
}