#pragma once

#include "Zenith/Core/Layer.hpp"

namespace Zenith {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		ImGuiLayer(const std::string& name);
		~ImGuiLayer() override;

		void Begin();
		void End();

		bool IsInputEnabled();
		void SetInputEnabled(bool enabled);

		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;
	private:
		float m_Time = 0.0f;
	};

}
