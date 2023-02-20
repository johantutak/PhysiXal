#pragma once

#include "physixal.h"

class Editor : public PhysiXal::Layer
{
public:
	// Editor
	Editor();
	virtual ~Editor();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(PhysiXal::Timestep ts) override;

	void OnEvent(PhysiXal::Event& e) override;
private:
};