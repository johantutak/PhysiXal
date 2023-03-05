#pragma once

#include "core/core.h"
#include "core/application.h"

extern PhysiXal::Application* PhysiXal::CreateApplication();

int main(int argc, char** argv)
{
	// Entry point (int main())
	PhysiXal::InitializeCore();

	PX_PROFILE_BEGIN_SESSION("Startup", "../profiling/startup.json");
	auto app = PhysiXal::CreateApplication();
	PX_PROFILE_END_SESSION();

	PX_PROFILE_BEGIN_SESSION("Runtime", "../profiling/runtime.json");
	app->Run();
	PX_PROFILE_END_SESSION();

	PX_PROFILE_BEGIN_SESSION("Shutdown", "../profiling/shutdown.json");
	delete app;
	PX_PROFILE_END_SESSION();

	PhysiXal::ShutdownCore();
}