#include "example_game.h"

ExampleGame::ExampleGame()
	: CoreLayer("ExampleGame")
{
	PX_PROFILE_FUNCTION();

	// Draw
}

ExampleGame::~ExampleGame()
{
	PX_PROFILE_FUNCTION();
}

void ExampleGame::OnAttach()
{
	PX_PROFILE_FUNCTION();
}

void ExampleGame::OnDetach()
{
	PX_PROFILE_FUNCTION();
}

void ExampleGame::OnUpdate(PhysiXal::Timestep ts)
{
	PX_PROFILE_FUNCTION();

	// Update

	// Render
}

void ExampleGame::OnEvent(PhysiXal::Event& e)
{
}