#include "znpch.hpp"
#include "Application.hpp"

#include <GLFW/glfw3.h>

namespace Zenith {

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationProps& props)
	{
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create({ props.Name, props.WindowWidth, props.WindowHeight }));
		m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });
		m_Window->SetVSync(false);
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

	void Application::Run()
	{
		OnInit();
		while (m_Running)
		{
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window->OnUpdate();
			OnUpdate();
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