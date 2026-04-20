#include "EditorLayer.hpp"
#include "Utilities/FileSystem.hpp"
#include "Utilities/CommandLineParser.hpp"

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
        // Persistent Storage
        {
            m_PersistentStoragePath = Zenith::FileSystem::GetPersistentStoragePath() / "Zenith-Editor";

            if (!Zenith::FileSystem::Exists(m_PersistentStoragePath))
                Zenith::FileSystem::CreateDirectory(m_PersistentStoragePath);
        }

        // Update the ZENITH_DIR environment variable every time we launch
        {
            auto workingDirectory = Zenith::FileSystem::GetWorkingDirectory();

            if (workingDirectory.stem().string() == "Zenith-Editor")
                workingDirectory = workingDirectory.parent_path();

            Zenith::FileSystem::SetEnvironmentVariable("ZENITH_DIR", workingDirectory.string());
        }
        
        PushLayer(new Zenith::EditorLayer());
    }
private:
    std::filesystem::path m_PersistentStoragePath;
};

Zenith::Application* Zenith::CreateApplication(int argc, char** argv)
{
    Zenith::CommandLineParser cli(argc, argv);

    auto cd = cli.GetOpt("C");
    if(!cd.empty()) {
        Zenith::FileSystem::SetWorkingDirectory(cd);
    }

    ApplicationSpecification specification;
    specification.Name = "Zenith-Editor";
    specification.WindowWidth = 1280;
    specification.WindowHeight = 720;
    specification.Mode = WindowMode::Windowed;

    return new ZenithEditorApplication(specification);
}