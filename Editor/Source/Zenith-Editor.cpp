#include "EntryPoint.hpp"

class ZenithEditorApplication : public Zenith::Application
{
public:
    ZenithEditorApplication(const Zenith::ApplicationSpecification& specification)
        : Application(specification)
    {
    }
};

Zenith::Application* Zenith::CreateApplication(int argc, char** argv)
{
    ApplicationSpecification specification;
    specification.name = "Zenith-Editor";
    specification.windowWidth = 1280;
    specification.windowHeight = 720;

    return new ZenithEditorApplication(specification);
}