#pragma once

#include "core/physixal.h"

// From Hazel Engine

#ifdef PX_PLATFORM_WINDOWS

extern PhysiXal::Application* PhysiXal::CreateApplication();

int main(int argc, char** argv)
{
	PhysiXal::Log::Init();
	PX_CORE_WARN("Init Log!");
	PX_INFO("Hello!");

	auto app = PhysiXal::CreateApplication();
	app->Run();
	delete app;
}

#endif