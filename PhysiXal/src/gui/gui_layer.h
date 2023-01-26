#pragma once

#include "core/core_layer.h"

#include "events/app_event.h"
#include "events/key_event.h"
#include "events/mouse_event.h"

// #### TEMPORARY ####
#include <vulkan/vulkan.h>

namespace PhysiXal {

	class GuiLayer : public CoreLayer
	{
	public:
		GuiLayer();
		~GuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		/*virtual void OnGuiRender() override;*/

		void Begin();
		void End();

		// #### TEMPORARY ####
		void GuiDescriptorPool();
		void GuiRenderPass();
	private:
		float m_Time = 0.0f;

		// #### TEMPORARY ####
		//VkDescriptorPool m_GuiPool;
		//VkRenderPass m_GuiRenderPass;
	};
}