#include <Zenith.hpp>
#include <Zenith/EntryPoint.hpp>

#include "EditorLayer.hpp"

class ZenithEditorApplication : public Zenith::Application
{
public:
	ZenithEditorApplication(const Zenith::ApplicationProps& props)
		: Application(props)
	{}

	virtual void OnInit() override
	{
		PushLayer(new Zenith::EditorLayer());
	}
};

Zenith::Application* Zenith::CreateApplication(int argv, char** argc)
{
	return new ZenithEditorApplication({ "ZenithEditor", 1600, 900 });
}