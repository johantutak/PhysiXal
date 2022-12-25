#include "core/physixal.h"
#include "core/entry_point.h"

#ifdef PX_PLATFORM_WINDOWS
	#include "example_game.h"
#endif
#ifdef PX_PLATFORM_LINUX
	#include "lnx_example_game.h"
#endif

class GameApp : public PhysiXal::Application
{
	public:
	GameApp(const PhysiXal::ApplicationSpecification& specification)
		: Application(specification)
	{
#ifdef PX_PLATFORM_WINDOWS
		PushLayer(new ExampleGame());
#endif
#ifdef PX_PLATFORM_LINUX
		PushLayer(new LnxExampleGame());
#endif
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