#include "core/physixal.h"
#include "core/entry_point.h"

#include "example_game.h"

class GameApp : public PhysiXal::Application
{
	public:
	GameApp()
	{
		PushLayer(new ExampleGame());
	}

	~GameApp()
	{

	}

};

PhysiXal::Application* PhysiXal::CreateApplication()
{
	return new GameApp();
}