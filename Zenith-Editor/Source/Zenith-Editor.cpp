#include <Zenith.hpp>
#include <Zenith/EntryPoint.hpp>

#include "EditorLayer.hpp"

class ZenithEditorApplication : public Zenith::Application
{
public:
	ZenithEditorApplication(const Zenith::ApplicationSpecification& specification)
		: Application(specification)
	{}

	virtual void OnInit() override
	{
		PushLayer(znew Zenith::EditorLayer());
	}
};

Zenith::Application* Zenith::CreateApplication(int argv, char** argc)
{
	Zenith::ApplicationSpecification specification;
	specification.Name = "Zenit-Editor";
	specification.WindowWidth = 1600;
	specification.WindowHeight = 900;
	specification.WindowDecorated = true;
	specification.StartMaximized = true;
	specification.VSync = true;

	return znew ZenithEditorApplication(specification);
}
