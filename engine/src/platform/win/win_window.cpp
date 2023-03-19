#include "px_pch.h"
#include "platform/win/win_window.h"

#include "events/app_event.h"

#include "core/application.h"

#include "api/vulkan/vk_renderer.h"

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Windows window
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
	static U8 s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		PX_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WinWindow::WinWindow(const WindowSpecification& props)
		: m_Specification(props)
	{
		PX_PROFILE_FUNCTION();
	}

	WinWindow::~WinWindow()
	{
		PX_PROFILE_FUNCTION();

		Shutdown();
	}

	void WinWindow::Init()
	{
		m_Data.Title = m_Specification.Title;
		m_Data.Width = m_Specification.Width;
		m_Data.Height = m_Specification.Height;

		PX_CORE_INFO("Creating window for {0} ({1}, {2})", m_Specification.Title, m_Specification.Width, m_Specification.Height);

		if (s_GLFWWindowCount == 0)
		{
			PX_PROFILE_SCOPE("glfwInit");

			int success = glfwInit();
			PX_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			PX_PROFILE_SCOPE("glfwCreateWindow");

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

			m_Window = glfwCreateWindow((int)m_Specification.Width, (int)m_Specification.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		glfwSetWindowUserPointer(m_Window, &m_Data);

		// Set GFLW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.Width = width;
		data.Height = height;

		WindowResizeEvent event(width, height);
		data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		WindowCloseEvent event;
		data.EventCallback(event);
			});
	}

	void WinWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount -= 1)
		{
			glfwTerminate();
		}

		PX_CORE_WARN("...Destroying window");
	}

	void WinWindow::OnUpdate()
	{
		glfwPollEvents();
	}
}