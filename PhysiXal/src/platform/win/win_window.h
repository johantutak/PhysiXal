#pragma once

#include "core/window.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

namespace PhysiXal {

	// From Hazel & Little Vulkan Engine

	class WinWindow : public Window
	{
	public:
		WinWindow(const WindowProps& props);
		virtual ~WinWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		
		//	#### TO DO ####	Set Context for vSync interval
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;

			//	#### TO DO ####	Set Context for vSync interval
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}