#pragma once

// #include "Core/layer.h"

// #include "Events/app_event.h"
// #include "Events/key_event.h"
// #include "Events/mouse_event.h"

namespace Hazel {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		/* virtual void OnImGuiRender() override; */

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}