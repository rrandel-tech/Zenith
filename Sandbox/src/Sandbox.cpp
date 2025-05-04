#include "Zenith.hpp"
#include "Zenith/EntryPoint.hpp"

class GameLayer : public Zenith::Layer
{
public:
	GameLayer()
	{}

	virtual ~GameLayer()
	{}

	virtual void OnAttach() override
	{}

	virtual void OnDetach() override
	{}

	virtual void OnUpdate() override
	{}

	virtual void OnEvent(Zenith::Event& event) override
	{}
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