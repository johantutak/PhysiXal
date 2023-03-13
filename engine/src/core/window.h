#pragma once

#include "core/core.h"
#include "events/event.h"

namespace PhysiXal {

	struct WindowSpecification
	{
		std::string Title = "PhysiXal Engine";
		U32 Width = 1600;
		U32 Height = 900;
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

		virtual U32 GetWidth() const = 0;
		virtual U32 GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
                
		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowSpecification& specification = WindowSpecification());
	};
}