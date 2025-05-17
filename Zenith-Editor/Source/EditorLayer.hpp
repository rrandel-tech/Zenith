#pragma once

#include "Zenith.hpp"

#include "Zenith/ImGui/ImGuiLayer.hpp"

namespace Zenith {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;

		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

		void UpdateWindowTitle(const std::string& sceneName);
	private:
		float m_ClearColor[4];
	};

}
