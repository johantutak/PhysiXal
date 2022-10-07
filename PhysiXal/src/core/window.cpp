#include "px_pch.h"

#include "core/window.h"

#ifdef PX_PLATFORM_WINDOWS
#include "platform/win/win_window.h"
#endif

namespace PhysiXal	{

	// From Hazel Engine

	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef PX_PLATFORM_WINDOWS
		return CreateScope<WinWindow>(props);
#else
		PX_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}