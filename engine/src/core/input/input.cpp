#include "px_pch.h"
#include "core/input/input.h"

#include "platform/win/win_input.h"

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Input
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
#ifdef PX_PLATFORM_WINDOWS
		return CreateScope<WindowsInput>();
#else
		PX_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}