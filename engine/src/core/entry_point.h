#pragma once

#include "core/core.h"
#include "core/application.h"
#include "debug/profiler/instrumentor.h"

#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

extern PhysiXal::Application* PhysiXal::CreateApplication();

int main(int argc, char** argv)
{
	// Entry point (int main())
	PhysiXal::Core::InitializeCore();

#if PX_PROFILE true
	// #### TO DO #### if profile is turned off, dont create any folders
	// Gives application current time
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::ostringstream oss;
	oss << std::put_time(&tm, "%H-%M-%S (hour-min-sec)");
	auto current_time = oss.str();

	// Create "the current date and time" directory if it doesn't exist
	std::string profileDirectory = "../profiling/" __DATE__ "/" + current_time;
	
	if (!std::filesystem::exists(profileDirectory))
	{
		std::filesystem::create_directories(profileDirectory);
	}
#endif

	// Start application 
	PX_PROFILE_BEGIN_SESSION("Startup", "../profiling/" __DATE__ "/" + current_time + "/startup.json");
	auto app = PhysiXal::CreateApplication();
	PX_PROFILE_END_SESSION();

	// Run application
	PX_PROFILE_BEGIN_SESSION("Runtime", "../profiling/" __DATE__ "/" + current_time + "/runtime.json");
	app->Run();
	PX_PROFILE_END_SESSION();

	// Shut down application
	PX_PROFILE_BEGIN_SESSION("Shutdown", "../profiling/" __DATE__ "/" + current_time + "/shutdown.json");
	pdelete app;
	PX_PROFILE_END_SESSION();

	PhysiXal::Core::ShutdownCore();
}