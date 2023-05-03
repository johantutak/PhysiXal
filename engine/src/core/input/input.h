#pragma once

#include "core/core.h"
#include "core/input/key_codes.h"
#include "core/input/mouse_codes.h"

namespace PhysiXal {

	class Input
	{
	protected:
		Input() = default;
	public:
		// Input
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;
	public:
		static bool IsKeyPressed(KeyCode key) { return s_Instance->IsKeyPressedImpl(key); }

		static bool IsMouseButtonPressed(MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		
		static void SetCursorMode(bool enabled) { return s_Instance->SetCursorModeImpl(enabled); }

		static Scope<Input> Create();
	protected:
		virtual bool IsKeyPressedImpl(KeyCode key) = 0;

		virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

		virtual void SetCursorModeImpl(bool enabled) = 0;
	private:
		static Scope<Input> s_Instance;
	};
}