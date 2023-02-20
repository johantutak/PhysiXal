#pragma once

#include "core/core.h"
#include "events/event.h"

namespace PhysiXal {

	struct WindowSpecification
	{
		std::string Title = "PhysiXal Engine";
		uint32_t Width = 1600;
		uint32_t Height = 900;
	};

	// Interface representing a desktop system based Window
	class Window
	{
	public:
		// Window
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void Init() = 0;
		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
                
		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowSpecification& specification = WindowSpecification());
	};
}