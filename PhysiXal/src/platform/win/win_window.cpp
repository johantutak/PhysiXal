#include "pch/px_pch.h"

#include "win_window.h"

namespace PhysiXal {

	WinWindow::WinWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name } {
		initWindow();
	}

	WinWindow::~WinWindow() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void WinWindow::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	}
}  