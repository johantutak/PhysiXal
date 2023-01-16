#include "core/physixal.h"
#include "core/entry_point.h"

#include "example_game.h"

class GameApp : public PhysiXal::Application
{
	public:
	GameApp(const PhysiXal::ApplicationSpecification& specification)
		: Application(specification)
	{
		PushLayer(new ExampleGame());
	}

	~GameApp()
	{
	}
};

PhysiXal::Application* PhysiXal::CreateApplication()
{
	PhysiXal::ApplicationSpecification specification;
	specification.Name = "PhysiXal Engine";
	specification.WindowWidth = 1600;
	specification.WindowHeight = 900;

	return new GameApp(specification);
}