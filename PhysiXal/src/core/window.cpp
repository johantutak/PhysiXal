#include "px_pch.h"
#include "core/window.h"

#ifdef PX_PLATFORM_WINDOWS
#include "platform/win/win_window.h"
#elif defined PX_PLATFORM_LINUX
#include "platform/lnx/lnx_window.h"
#endif

namespace PhysiXal	{

	// From Hazel Engine

    Window* Window::Create(const WindowSpecification& specification)
	{
#ifdef PX_PLATFORM_WINDOWS
        return new WinWindow(specification);
#elif defined PX_PLATFORM_LINUX
        return new LnxWindow(specification);
#else
    PX_CORE_ASSERT(false, "Unknown platform!");
    return nullptr;
#endif
	}
}