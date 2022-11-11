#pragma once

#include "core/core.h"

// From Hazel Engine

#ifdef PX_PLATFORM_WINDOWS

extern PhysiXal::Application* PhysiXal::CreateApplication();

int main(int argc, char** argv)
{
	PhysiXal::Log::Init();
	PX_CORE_WARN("Initializing Log (core).");
	PX_INFO("Initializing Applictaion (client).");

	auto app = PhysiXal::CreateApplication();
	app->Run();
	delete app;
}

#endif