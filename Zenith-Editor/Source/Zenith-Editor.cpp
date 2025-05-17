#include "EditorLayer.hpp"
#include "Zenith/Utilities/FileSystem.hpp"

#include "Zenith/EntryPoint.hpp"

class ZenithEditorApplication : public Zenith::Application
{
public:
	ZenithEditorApplication(const Zenith::ApplicationSpecification& specification)
		: Application(specification)
	{}

	virtual void OnInit() override
	{
		// Persistent Storage
		{
			m_PersistentStoragePath = Zenith::FileSystem::GetPersistentStoragePath() / "Zenith-Editor";

			if (!Zenith::FileSystem::Exists(m_PersistentStoragePath))
				Zenith::FileSystem::CreateDirectory(m_PersistentStoragePath);
		}

		// Update the ZENITH_DIR config entry every time we launch
		{
			auto workingDirectory = Zenith::FileSystem::GetWorkingDirectory();

			if (workingDirectory.stem().string() == "Zenith-Editor")
				workingDirectory = workingDirectory.parent_path();

			Zenith::FileSystem::SetConfigValue("ZENITH_DIR", workingDirectory.string());
		}

		PushLayer(znew Zenith::EditorLayer());
	}

private:
	std::filesystem::path m_PersistentStoragePath;
};

Zenith::Application* Zenith::CreateApplication(int argv, char** argc)
{
	Zenith::ApplicationSpecification specification;
	specification.Name = "Zenith-Editor";
	specification.WindowWidth = 1600;
	specification.WindowHeight = 900;
	specification.WindowDecorated = true;
	specification.StartMaximized = true;
	specification.VSync = true;
	specification.IconPath = "Resources/Editor/Zenith.png";

	return znew ZenithEditorApplication(specification);
}
