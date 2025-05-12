#include "znpch.hpp"
#include "Application.hpp"

#include "Zenith/Renderer/Renderer.hpp"
#include "Zenith/Renderer/Framebuffer.hpp"

#include "FatalSignal.hpp"

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <Windows.h>

namespace Zenith {

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationProps& props)
	{
		FatalSignal::Install();

		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create({ props.Name, props.WindowWidth, props.WindowHeight }));
		m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });
		m_Window->SetVSync(true);

		m_ImGuiLayer = new ImGuiLayer("ImGui");
		PushOverlay(m_ImGuiLayer);

		Renderer::Init();
		Renderer::WaitAndRender();
	}

	Application::~Application()
	{
		m_Window->SetEventCallback([](Event& e) {});

		for (Layer* layer : m_LayerStack)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.PopLayer(layer);
		layer->OnDetach();
	}

	void Application::PopOverlay(Layer* layer)
	{
		m_LayerStack.PopOverlay(layer);
		layer->OnDetach();
	}

	void Application::RenderImGui()
	{
		m_ImGuiLayer->Begin();

		ImGui::Begin("Renderer");
		auto& caps = RendererAPI::GetCapabilities();
		ImGui::Text("Vendor: %s", caps.Vendor.c_str());
		ImGui::Text("Renderer: %s", caps.Renderer.c_str());
		ImGui::Text("Version: %s", caps.Version.c_str());
		ImGui::Text("Frame Time: %.2fms\n", m_TimeStep.GetMilliseconds());
		ImGui::End();

		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();

		m_ImGuiLayer->End();
	}

	void Application::Run()
	{
		OnInit();
		while (m_Running)
		{
			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(m_TimeStep);

				// Render ImGui on render thread
				Application* app = this;
				Renderer::Submit([app]() { app->RenderImGui(); });

				Renderer::WaitAndRender();
			}
			m_Window->OnUpdate();

			float time = GetTime();
			m_TimeStep = time - m_LastFrameTime;
			m_LastFrameTime = time;
		}
		OnShutdown();
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return OnWindowResize(e); });
		dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return OnWindowClose(e); });

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
				break;
		}

		if (event.Handled)
			return;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		int width = e.GetWidth(), height = e.GetHeight();
		if (width == 0 || height == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::Submit([=]() { glViewport(0, 0, width, height); });
		auto& fbs = FramebufferPool::GetGlobal()->GetAll();
		for (auto& fb : fbs)
			fb->Resize(width, height);

		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent&)
	{
		m_Running = false;
		return true;
	}

	std::string Application::OpenFile(const char* filter) const
	{
		OPENFILENAMEA ofn;       // common dialog box structure
		CHAR szFile[260] = { 0 };       // if using TCHAR macros

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)m_Window->GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}

	std::string Application::SaveFile(const char* filter) const
	{
		OPENFILENAMEA ofn;       // common dialog box structure
		CHAR szFile[260] = { 0 };       // if using TCHAR macros

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)m_Window->GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}

	float Application::GetTime() const
	{
		return (float)glfwGetTime();
	}

	const char* Application::GetConfigurationName()
	{
#if defined(ZN_DEBUG)
		return "Debug";
#elif defined(ZN_RELEASE)
		return "Release";
#elif defined(ZN_DIST)
		return "Dist";
#else
#error Undefined configuration?
#endif
	}

	const char* Application::GetPlatformName()
	{
#if defined(ZN_PLATFORM_WINDOWS)
		return "Windows x64";
#else
#error Undefined platform?
#endif
	}

}