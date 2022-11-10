#pragma once

#include "core/physixal.h"

class ExampleGame : public PhysiXal::CoreLayer
{
public:
	ExampleGame();
	virtual ~ExampleGame() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate() override;

	void OnEvent(PhysiXal::Event& e) override;
private:
};