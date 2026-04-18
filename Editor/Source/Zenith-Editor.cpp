#include "EditorLayer.hpp"

#include "EntryPoint.hpp"

class ZenithEditorApplication : public Zenith::Application
{
public:
    ZenithEditorApplication(const Zenith::ApplicationSpecification& specification)
        : Application(specification)
    {
    }

    virtual void OnInit() override
    {
        PushLayer(new Zenith::EditorLayer());
    }
};

Zenith::Application* Zenith::CreateApplication(int argc, char** argv)
{
    ApplicationSpecification specification;
    specification.Name = "Zenith-Editor";
    specification.WindowWidth = 1280;
    specification.WindowHeight = 720;

    return new ZenithEditorApplication(specification);
}