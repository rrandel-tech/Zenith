#include "znpch.hpp"
#include "Application.hpp"

namespace Zenith {

#define BIND_EVENT_FN(fn) std::bind(&Application::fn, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationProps& props)
	{
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create({ props.Name, props.WindowWidth, props.WindowHeight }));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		m_Window->SetVSync(false);
	}

	Application::~Application() = default;

	void Application::Run()
	{
		OnInit();

		while (m_Running)
		{
			m_Window->OnUpdate();
			OnUpdate();
		}

		OnShutdown();
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
	}

	bool Application::OnWindowResize(WindowResizeEvent&)
	{
		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent&)
	{
		m_Running = false;
		return true;
	}

}