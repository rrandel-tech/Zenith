#pragma once

#include "Zenith/Core/Base.hpp"
#include "Zenith/Core/Assert.hpp"
#include "Layer.hpp"

#include <vector>

namespace Zenith {

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		Layer* operator[](size_t index)
		{
			ZN_CORE_ASSERT(index < m_Layers.size(), "Layer index out of bounds");
			return m_Layers[index];
		}

		const Layer* operator[](size_t index) const
		{
			ZN_CORE_ASSERT(index < m_Layers.size(), "Layer index out of bounds");
			return m_Layers[index];
		}

		size_t Size() const { return m_Layers.size(); }

		auto begin() { return m_Layers.begin(); }
		auto end() { return m_Layers.end(); }
		auto begin() const { return m_Layers.begin(); }
		auto end()   const { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		uint32_t m_LayerInsertIndex = 0;
	};

}
