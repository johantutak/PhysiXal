#pragma once

#include "core/input/input.h"

namespace PhysiXal {
    
	class WindowsInput : public Input
	{
	protected:
		// Windows input
		virtual bool IsKeyPressedImpl(KeyCode key) override;

		virtual bool IsMouseButtonPressedImpl(MouseCode button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}