#include "znpch.hpp"
#include "ImGuiLayer.hpp"

#include "imgui.h"

#define IMGUI_IMPL_API
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Zenith/Core/Application.hpp"
#include "Zenith/Renderer/Renderer.hpp"

#include <GLFW/glfw3.h>

namespace Zenith {

	ImGuiLayer::ImGuiLayer() = default;

	ImGuiLayer::ImGuiLayer(const std::string& name)
		: Layer(name)
	{}

	ImGuiLayer::~ImGuiLayer() = default;

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		// Enable useful features
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Keyboard nav
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Multi-viewport

		ImFont* pFont = io.Fonts->AddFontFromFileTTF("Resources/Fonts/Inter/Inter_18pt-Regular.ttf", 18.0f);
		io.FontDefault = io.Fonts->Fonts.back();

		// Apply dark theme
		ImGui::StyleColorsDark();

		// Match platform windows to main window style
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, style.Colors[ImGuiCol_WindowBg].w);

		// Setup platform/renderer bindings
		auto& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 450");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		auto& app = Application::Get();

		io.DisplaySize = ImVec2(
			static_cast<float>(app.GetWindow().GetWidth()),
			static_cast<float>(app.GetWindow().GetHeight())
		);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Handle multi-viewport rendering
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_context);
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{}

}