#include "EditorLayer.hpp"

#include "Zenith/Debug/Profiler.hpp"

#include "Zenith/Utilities/FileSystem.hpp"

#include <GLFW/include/GLFW/glfw3.h>

#include <imgui/imgui_internal.h>

#include <filesystem>

namespace Zenith {

	EditorLayer::EditorLayer()
	{}

	EditorLayer::~EditorLayer()
	{}

	void EditorLayer::OnAttach()
	{
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
		ZN_PROFILE_FUNC();
	}

	void EditorLayer::OnImGuiRender()
	{
		ZN_PROFILE_FUNC();

		ImGui::Begin("Settings");
		Application& app = Application::Get();
		bool vsync = app.GetWindow().IsVSync();
		bool changed = ImGui::Checkbox("##Vsync", &vsync);
		ImGui::SameLine();
		ImGui::TextUnformatted("VSync");
		if (changed)
			app.GetWindow().SetVSync(vsync);

		if (ImGui::Button("Open File Dialog"))
		{
			std::filesystem::path filePath = Zenith::FileSystem::OpenFileDialog({ {"All Files", "*.*"} });

			if (!filePath.empty())
			{
				ZN_INFO("Selected file: {0}", filePath.string());
			}
			else
			{
				ZN_ERROR("No file selected.");
			}
		}

		ImGui::End();

		// Controller Test Window
		ImGui::Begin("Controller Test");

		static int controllerID_UI = 1;
		static int buttonID = 0;
		static int axisID = 0;

		ImGui::InputInt("Controller ID", &controllerID_UI);
		ImGui::InputInt("Button ID", &buttonID);
		ImGui::InputInt("Axis ID", &axisID);

		int controllerID = controllerID_UI - 1;

		if (Input::IsControllerPresent(controllerID))
		{
			bool isDown = Input::IsControllerButtonDown(controllerID, buttonID);
			float axisValue = Input::GetControllerAxis(controllerID, axisID);

			ImGui::Text("Button %d: %s", buttonID, isDown ? "Pressed" : "Released");
			ImGui::Text("Axis %d: %.3f", axisID, axisValue);
		}
		else
		{
			ImGui::TextColored(ImVec4(1, 0.5f, 0.5f, 1), "Controller %d not connected", controllerID_UI);
		}

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{}

}
