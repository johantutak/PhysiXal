#pragma once

#include "core/window.h"

#include <GLFW/glfw3.h>

namespace PhysiXal {

	// From Hazel & Little Vulkan Engine
    
#ifdef PX_PLATFORM_LINUX
    
	class LnxWindow : public Window
	{
	public:
		LnxWindow(const WindowSpecification& specification);
		virtual ~LnxWindow();

		virtual void Init() override;
		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
                
        // ####	Initializing vSync for PysiXal with OpenGL API	####
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
                
		virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		WindowSpecification m_Specification;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;

			// ####	Initializing vSync for PysiXal with OpenGL API	####
			bool VSync;
			
			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
#endif
}