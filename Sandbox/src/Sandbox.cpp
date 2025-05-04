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
	{}

	virtual void OnDetach() override
	{}

	virtual void OnUpdate() override
	{
		Zenith::Renderer::Clear(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
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