#pragma once

#include "core/core.h"
#include "core/timestep.h"
#include "events/event.h"

namespace PhysiXal {

	// From Hazel & Walnut Engine

	// Graphics, Events & Update logic.
	// Next up is the LayerStack class. (Ordered list of layers = layer stack).
	// Layers determine what order things are drawn in and so on.

	class Layer
	{
	public:
		// Layer
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate(Timestep ts) {}

		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}