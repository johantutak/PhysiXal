#pragma once

#include "core/core.h"

// From Hazel Engine

extern PhysiXal::Application* PhysiXal::CreateApplication();

int main(int argc, char** argv)
{
	PhysiXal::Log::Init();
	PX_CORE_INFO("Initializing Log (core).");
	PX_INFO("Initializing Application (client).");

	auto app = PhysiXal::CreateApplication();
	app->Run();
	delete app;
}