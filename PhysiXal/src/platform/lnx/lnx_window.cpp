#include "px_pch.h"
#include "platform/lnx/lnx_window.h"

#include "events/app_event.h"

#ifdef PX_PLATFORM_LINUX
	#include <glad/glad.h>
#endif

namespace PhysiXal {

	// From Hazel & Little Vulkan Engine
    
#ifdef PX_PLATFORM_LINUX
    
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		PX_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	LnxWindow::LnxWindow(const WindowSpecification& props)
		: m_Specification(props)
	{
	}

	LnxWindow::~LnxWindow()
	{
		Shutdown();
	}

	void LnxWindow::Init()
	{
		m_Data.Title = m_Specification.Title;
		m_Data.Width = m_Specification.Width;
		m_Data.Height = m_Specification.Height;

		PX_CORE_INFO("Creating window {0} ({1}, {2})", m_Specification.Title, m_Specification.Width, m_Specification.Height);

		if (s_GLFWWindowCount == 0)
		{
			int success = glfwInit();
			PX_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			// #### TEMPORARY #### TO DO #### Move this to it's own class ####
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			// #### ENDS ####
			
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

			m_Window = glfwCreateWindow((int)m_Specification.Width, (int)m_Specification.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		glfwSetWindowUserPointer(m_Window, &m_Data);

		// ####	OpenGL context is created within the window class ####
		PX_CORE_INFO("Creating OpenGL context");
		glfwMakeContextCurrent(m_Window);
		
		// ####	Initializing vSync for PysiXal with OpenGL API	####
		SetVSync(true);
                
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

	void LnxWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount -= 1)
		{
			glfwTerminate();
		}
                
        PX_CORE_WARN("...Destroying window");
	}

	void LnxWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}
        
	// ####	Initializing vSync for PysiXal with OpenGL API.	####
	void LnxWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool LnxWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
#endif
}