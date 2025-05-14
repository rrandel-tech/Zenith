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
		Ref<VertexBuffer> m_VB;
		Ref<IndexBuffer> m_IB;
		float m_ClearColor[4];
	};

}
