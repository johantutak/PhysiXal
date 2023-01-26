#pragma once

#include "core/physixal.h"

class ExampleGame : public PhysiXal::CoreLayer
{
public:
	ExampleGame();
	virtual ~ExampleGame();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(PhysiXal::Timestep ts) override;

	virtual void OnGuiRender() override;

	void OnEvent(PhysiXal::Event& e) override;
private:
};