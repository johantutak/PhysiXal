#pragma once

#include "core/core.h"

// From Hazel Engine

extern PhysiXal::Application* PhysiXal::CreateApplication();

int main(int argc, char** argv)
{
	PhysiXal::Log::Init();
	PX_CORE_INFO("initializing Log (core).");
	PX_INFO("initializing Application (client).");

	auto app = PhysiXal::CreateApplication();
	app->Run();
	delete app;
}