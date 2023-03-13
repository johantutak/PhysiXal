#include "px_pch.h"
#include "core/window.h"

#include "platform/win/win_window.h"

namespace PhysiXal	{

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Window
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Window* Window::Create(const WindowSpecification& specification)
	{
#ifdef PX_PLATFORM_WINDOWS
        return pnew WinWindow(specification);
#else
    PX_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
	}
}