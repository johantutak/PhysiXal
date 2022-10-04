#pragma once

#include "core/core.h"
#include "core/core_layer.h"

#include <vector>

namespace PhysiXal {

	// From Hazel Engine

	class LayerStack
	{
	public:
		LayerStack() = default;;
		~LayerStack();

		void PushLayer(CoreLayer* layer);
		void PushOverlay(CoreLayer* overlay);
		void PopLayer(CoreLayer* layer);
		void PopOverlay(CoreLayer* overlay);

		std::vector<CoreLayer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<CoreLayer*>::iterator end() { return m_Layers.end(); }
		std::vector<CoreLayer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<CoreLayer*>::reverse_iterator rend() { return m_Layers.rend(); }

		std::vector<CoreLayer*>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<CoreLayer*>::const_iterator end()	const { return m_Layers.end(); }
		std::vector<CoreLayer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<CoreLayer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }
	private:
		std::vector<CoreLayer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}