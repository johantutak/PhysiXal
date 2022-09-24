#pragma once

#define GLFW_INCLUDE_VULKAN
#include "/dev/ThePhysiXalEngine/PhysiXal/thirdparty/GLFW/include/GLFW/glfw3.h"

#include <string>

namespace PhysiXal {

	class WinWindow {
	public:
		WinWindow(int w, int h, std::string name);
		~WinWindow();

		WinWindow(const WinWindow&) = delete;
		WinWindow& operator=(const WinWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }

	private:
		void initWindow();

		const int width;
		const int height;

		std::string windowName;
		GLFWwindow* window;
	};
}