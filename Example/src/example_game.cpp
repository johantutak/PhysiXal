#include "example_game.h"

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

void ExampleGame::OnGuiRender()
{
	//ImGui::Begin("Settings");

	//ImGui::End();
}

void ExampleGame::OnEvent(PhysiXal::Event& e)
{
}