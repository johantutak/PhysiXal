#include "physixal.h"
#include "core/entry_point.h"

#include "editor.h"

class EditorApplication : public PhysiXal::Application
{
public:
	// Game application
	EditorApplication(const PhysiXal::ApplicationSpecification& specification)
		: Application(specification)
	{
		PushLayer(new Editor());
	}

	~EditorApplication()
	{
	}
};

PhysiXal::Application* PhysiXal::CreateApplication()
{
	PX_PROFILE_FUNCTION();

	PhysiXal::ApplicationSpecification specification;
	specification.Name = "PhysiXal Engine";
	specification.WindowWidth = 1600;
	specification.WindowHeight = 900;

	return new EditorApplication(specification);
}