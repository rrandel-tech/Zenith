#include "Zenith.hpp"
#include "Zenith/EntryPoint.hpp"

#include "Zenith/ImGui/ImGuiLayer.hpp"

class GameLayer : public Zenith::Layer
{
public:
	GameLayer() : m_ClearColor{ 0.2f, 0.3f, 0.8f, 1.0f }
	{}

	virtual ~GameLayer()
	{}

	virtual void OnAttach() override
	{
		static float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		static uint32_t indices[] = {
			0, 1, 2
		};

		m_VB = std::unique_ptr<Zenith::VertexBuffer>(Zenith::VertexBuffer::Create());
		m_VB->SetData(vertices, sizeof(vertices));

		m_IB = std::unique_ptr<Zenith::IndexBuffer>(Zenith::IndexBuffer::Create());
		m_IB->SetData(indices, sizeof(indices));

		m_Shader.reset(Zenith::Shader::Create("Resources/Shaders/shader.glsl"));
	}

	virtual void OnDetach() override
	{}

	virtual void OnUpdate() override
	{
		using namespace Zenith;
		Renderer::Clear(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);

		m_Shader->Bind();
		m_VB->Bind();
		m_IB->Bind();
		Renderer::DrawIndexed(3);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("GameLayer");
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
		ImGui::End();
	}

	virtual void OnEvent(Zenith::Event& event) override
	{}
private:
	std::unique_ptr<Zenith::VertexBuffer> m_VB;
	std::unique_ptr<Zenith::IndexBuffer> m_IB;
	std::unique_ptr<Zenith::Shader> m_Shader;
	float m_ClearColor[4];
};

class Sandbox : public Zenith::Application
{
public:
	Sandbox(const Zenith::ApplicationProps& props)
		: Application(props)
	{
		ZN_TRACE("Hello!");
	}

	virtual void OnInit() override
	{
		PushLayer(new GameLayer());
	}
};

Zenith::Application* Zenith::CreateApplication(int argc, char** argv)
{
	return new Sandbox({ "Sandbox", 1600, 900 });
}