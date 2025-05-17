#include "znpch.hpp"
#include "Application.hpp"

#include "Zenith/Renderer/Renderer.hpp"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include "Input.hpp"
#include "FatalSignal.hpp"

#include "imgui/imgui_internal.h"

#include "Zenith/Utilities/StringUtils.hpp"
#include "Zenith/Debug/Profiler.hpp"

#include <filesystem>
#include <nfd.hpp>

#include "Memory.hpp"

extern bool g_ApplicationRunning;
namespace Zenith {

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		FatalSignal::Install();

		//m_RenderDoc.Initialize();

		s_Instance = this;

		if (!specification.WorkingDirectory.empty())
			std::filesystem::current_path(specification.WorkingDirectory);

		m_Profiler = znew PerformanceProfiler();

		WindowSpecification windowSpec;
		windowSpec.Title = specification.Name;
		windowSpec.Width = specification.WindowWidth;
		windowSpec.Height = specification.WindowHeight;
		windowSpec.Decorated = specification.WindowDecorated;
		windowSpec.Fullscreen = specification.Fullscreen;
		windowSpec.VSync = specification.VSync;
		windowSpec.IconPath = specification.IconPath;
		m_Window = std::unique_ptr<Window>(Window::Create(windowSpec));
		m_Window->Init();
		m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });

		ZN_CORE_VERIFY(NFD::Init() == NFD_OKAY);

		// Renderer::Init();
		// Renderer::WaitAndRender();

		if (specification.StartMaximized)
			m_Window->Maximize();
		else
			m_Window->CenterWindow();
		m_Window->SetResizable(specification.Resizable);

		/*if (m_Specification.EnableImGui)
		{
			m_ImGuiLayer = ImGuiLayer::Create();
			PushOverlay(m_ImGuiLayer);
		}*/
	}

	Application::~Application()
	{
		NFD::Quit();

		m_Window->SetEventCallback([](Event& e) {});

		for (Layer* layer : m_LayerStack)
		{
			layer->OnDetach();
			delete layer;
		}

		//Renderer::Shutdown();

		delete m_Profiler;
		m_Profiler = nullptr;
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
		ZN_PROFILE_FUNC();
		ZN_SCOPE_PERF("Application::RenderImGui");

		m_ImGuiLayer->Begin();

		ImGui::Begin("Renderer");
		auto& caps = Renderer::GetCapabilities();
		ImGui::Text("Vendor: %s", caps.Vendor.c_str());
		ImGui::Text("Renderer: %s", caps.Device.c_str());
		ImGui::Text("Version: %s", caps.Version.c_str());
		ImGui::Text("Frame Time: %.2fms\n", m_TimeStep.GetMilliseconds());
		ImGui::End();

		for (int i = 0; i < m_LayerStack.Size(); i++)
			m_LayerStack[i]->OnImGuiRender();
	}

	void Application::Run()
	{
		OnInit();
		while (m_Running)
		{
			static uint64_t frameCounter = 0;

			ProcessEvents();

			m_Profiler->Clear();

			if (!m_Minimized)
			{
				// Renderer::BeginFrame();

				ZN_SCOPE_PERF("Application Layer::OnUpdate");
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(m_TimeStep);

				// Render ImGui on render thread
				/*Application* app = this;
				if (m_Specification.EnableImGui)
				{
					Renderer::Submit([app]() {app->RenderImGui(); });
					Renderer::Submit([=]() { m_ImGuiLayer->End(); });
				}
				Renderer::EndFrame();

				// On Render thread
				m_Window->GetRenderContext()->BeginFrame();
				Renderer::WaitAndRender();*/
				m_Window->SwapBuffers();

				// m_RenderDoc.SetCaptureName("Startup");

				int keys[1] = { GLFW_KEY_C };
				m_RenderDoc.SetCaptureKeys(keys, 1);

				// TODO: This should be in the render thread
				// Renderer::SwapQueues();
			}

			Input::ClearReleasedKeys();

			float time = GetTime();
			m_Frametime = time - m_LastFrameTime;
			m_TimeStep = glm::min<float>(m_Frametime, 0.0333f);
			m_LastFrameTime = time;

			frameCounter++;

			ZN_PROFILE_MARK_FRAME;
		}
		
		OnShutdown();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnShutdown()
	{
		m_EventCallbacks.clear();
		g_ApplicationRunning = false;
	}

	void Application::ProcessEvents()
	{
		Input::TransitionPressedKeys();
		Input::TransitionPressedButtons();

		m_Window->ProcessEvents();
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return OnWindowResize(e); });
		dispatcher.Dispatch<WindowMinimizeEvent>([this](WindowMinimizeEvent& e) { return OnWindowMinimize(e); });
		dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return OnWindowClose(e); });

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
				break;
		}

		if (event.Handled)
			return;

		for (auto& eventCallback : m_EventCallbacks)
		{
			eventCallback(event);

			if (event.Handled)
				break;
		}
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		const uint32_t width = e.GetWidth(), height = e.GetHeight();
		if (width == 0 || height == 0)
		{
			return false;
		}

		// m_Window->GetRenderContext()->OnResize(width, height);

		return false;
	}

	bool Application::OnWindowMinimize(WindowMinimizeEvent& e)
	{
		m_Minimized = e.IsMinimized();
		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		Close();
		return false;
	}

	float Application::GetTime() const
	{
		return (float)glfwGetTime();
	}

	const char* Application::GetConfigurationName()
	{
		return ZN_BUILD_CONFIG_NAME;
	}

	const char* Application::GetPlatformName()
	{
		return ZN_BUILD_PLATFORM_NAME;
	}

}
