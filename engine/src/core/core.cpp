#include "px_pch.h"
#include "core/core.h"

#include "log/log.h"

#define PX_BUILD_ID "v0.03"

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Core
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Core::InitializeCore()
	{
		Log::Init();

		PX_CORE_TRACE("PhysiXal Engine {}", PX_BUILD_ID);
		PX_CORE_TRACE("Initializing...");
		
		PX_CORE_INFO("Log (core)");
		PX_INFO("Application (client)");
	}

	void Core::ShutdownCore()
	{
		PX_CORE_TRACE("...Shutting down");

		PX_WARN("...Application (client)");
		PX_CORE_WARN("...Log (core)");

		Log::Shutdown();
	}
}