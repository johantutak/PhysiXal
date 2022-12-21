#pragma once

#include "core/window.h"

#include "GLFW/glfw3.h"

namespace PhysiXal {

	// From Hazel & Little Vulkan Engine
    
#ifdef PX_PLATFORM_WINDOWS
    
	class WinWindow : public Window
	{
	public:
		WinWindow(const WindowSpecification& specification);
		virtual ~WinWindow();

		virtual void Init() override;
		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

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

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
#endif
}