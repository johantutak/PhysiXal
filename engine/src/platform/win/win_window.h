#pragma once

#include "core/window.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

namespace PhysiXal {
    
	class WinWindow : public Window
	{
	public:
		// Windows window
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
}