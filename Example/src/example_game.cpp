#include "example_game.h"

#ifdef PX_PLATFORM_WINDOWS

ExampleGame::ExampleGame()
	: CoreLayer("ExampleGame")
{
	// Draw

}

ExampleGame::~ExampleGame()
{
}

void ExampleGame::OnAttach()
{
}

void ExampleGame::OnDetach()
{
}

void ExampleGame::OnUpdate(PhysiXal::Timestep ts)
{
	// Update

	// Render

}

void ExampleGame::OnEvent(PhysiXal::Event& e)
{
}
#endif
