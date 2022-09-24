#include "core/physixal.h"
#include "core/entry_point.h"

class Example : public PhysiXal::Application
{
	public:
	Example()
	{

	}

	~Example()
	{

	}

};

PhysiXal::Application* PhysiXal::CreateApplication()
{
	return new Example();
}