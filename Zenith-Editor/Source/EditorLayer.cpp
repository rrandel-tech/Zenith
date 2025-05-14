#include "EditorLayer.hpp"

namespace Zenith {

	EditorLayer::EditorLayer() 
		: m_ClearColor{ 0.2f, 0.3f, 0.8f, 1.0f }
	{}

	EditorLayer::~EditorLayer()
	{}

	void EditorLayer::OnAttach()
	{
		static float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		static uint32_t indices[] = {
			0, 1, 2
		};

		m_VB = Ref<VertexBuffer>(VertexBuffer::Create());
		m_VB->SetData(vertices, sizeof(vertices));

		m_IB = Ref<IndexBuffer>(Zenith::IndexBuffer::Create());
		m_IB->SetData(indices, sizeof(indices));

		UpdateWindowTitle("Untitled Project");
	}

	void EditorLayer::OnDetach()
	{}

	void EditorLayer::UpdateWindowTitle(const std::string& sceneName)
	{
		std::string title = sceneName + " - Zenith-Editor - " + Application::GetPlatformName() + " (" + Application::GetConfigurationName() + ")";
		Application::Get().GetWindow().SetTitle(title);
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		Renderer::Clear(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);

		m_VB->Bind();
		m_IB->Bind();
		Renderer::DrawIndexed(3);
	}

	void EditorLayer::OnImGuiRender()
	{
		ImGui::Begin("EditorLayer");
		ImGui::ColorEdit4("Clear Color", m_ClearColor);

		ImGui::Separator();

		Application& app = Application::Get();
		bool vsync = app.GetWindow().IsVSync();
		bool changed = ImGui::Checkbox("##Vsync", &vsync);
		ImGui::SameLine();
		ImGui::TextUnformatted("VSync");
		if (changed)
			app.GetWindow().SetVSync(vsync);
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{}

}
