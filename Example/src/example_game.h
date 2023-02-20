#pragma once

#include "physixal.h"

class ExampleGame : public PhysiXal::Layer
{
public:
	// Example game
	ExampleGame();
	virtual ~ExampleGame();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(PhysiXal::Timestep ts) override;

	void OnEvent(PhysiXal::Event& e) override;
private:
};