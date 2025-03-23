#include "znpch.hpp"
#include "Application.hpp"

#include <GLFW/glfw3.h>

namespace Zenith {

  #define BIND_EVENT_FN(fn) std::bind(&Application::##fn, this, std::placeholders::_1)

  Application::Application()
  {
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
  }

  Application::~Application()
  {}

  void Application::PushLayer(Layer* layer)
  {
    m_LayerStack.PushLayer(layer);
  }

  void Application::PushOverlay(Layer* layer)
  {
    m_LayerStack.PushOverlay(layer);
  }

  void Application::Run()
  {
    OnInit();
    while (m_Running)
    {
      for (Layer* layer : m_LayerStack)
        layer->OnUpdate();

      m_Window->OnUpdate();
    }
    OnShutdown();
  }

  void Application::OnEvent(Event& event)
  {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
    {
      (*--it)->OnEvent(event);
      if (event.Handled)
        break;
    }
  }

  bool Application::OnWindowResize(WindowResizeEvent& e)
  {
    return false;
  }

  bool Application::OnWindowClose(WindowCloseEvent& e)
  {
    m_Running = false;
    return true;
  }
}