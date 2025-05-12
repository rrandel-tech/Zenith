#pragma once

#include "znpch.hpp"
#include "Zenith/Core/Layer.hpp"

namespace Zenith {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		explicit ImGuiLayer(const std::string& name);
		~ImGuiLayer() override;

		void Begin();
		void End();

		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;
	private:
		float m_Time = 0.0f;
	};

}