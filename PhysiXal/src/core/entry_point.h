#pragma once

#include "core/core.h"
#include "core/application.h"

extern PhysiXal::Application* PhysiXal::CreateApplication();

int main(int argc, char** argv)
{
	// Entry point (int main())
	PhysiXal::Log::Init();
	PX_CORE_INFO("Initializing Log (core).");
	PX_INFO("Initializing Application (client).");

#ifdef PX_PLATFORM_WINDOWS
	PX_PROFILE_BEGIN_SESSION("Startup", "../profiling/startup.json", );
#endif
#ifdef PX_PLATFORM_LINUX
	PX_PROFILE_BEGIN_SESSION("Startup", "../../../profiling/startup.json");
#endif
	auto app = PhysiXal::CreateApplication();
	PX_PROFILE_END_SESSION();

#ifdef PX_PLATFORM_WINDOWS
	PX_PROFILE_BEGIN_SESSION("Runtime", "../profiling/runtime.json");
#endif
#ifdef PX_PLATFORM_LINUX
	PX_PROFILE_BEGIN_SESSION("Runtime", "../../../profiling/runtime.json");
#endif
	app->Run();
	PX_PROFILE_END_SESSION();

#ifdef PX_PLATFORM_WINDOWS
	PX_PROFILE_BEGIN_SESSION("Shutdown", "../profiling/shutdown.json");
#endif
#ifdef PX_PLATFORM_LINUX
	PX_PROFILE_BEGIN_SESSION("Shutdown", "../../../profiling/shutdown.json");
#endif
	delete app;
	PX_PROFILE_END_SESSION();
}