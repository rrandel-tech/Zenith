#include "znpch.hpp"
#include "Application.hpp"

namespace Zenith {

  #define BIND_EVENT_FN(fn) std::bind(&Application::##fn, this, std::placeholders::_1)

  Application::Application()
  {
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
  }

  Application::~Application()
  {}

  void Application::Run()
  {
    while (m_Running)
    {
      m_Window->OnUpdate();
    }
  }

  void Application::OnEvent(Event& event)
  {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

    ZN_CORE_TRACE("{}", event.ToString());
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