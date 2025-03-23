#pragma once

#include "Zenith/Core/Base.hpp"
#include "Zenith/Core/Window.hpp"

#include "Zenith/Core/Events/ApplicationEvent.hpp"

namespace Zenith {

  class Application
  {
  public:
    Application();
    virtual ~Application();

    void Run();

    virtual void OnInit() {}
    virtual void OnShutdown() {}
    virtual void OnUpdate() {}

    virtual void OnEvent(Event& event);
  private:
    bool OnWindowResize(WindowResizeEvent& e);
    bool OnWindowClose(WindowCloseEvent& e);
  private:
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
  };

  // Implemented by CLIENT
  Application* CreateApplication();
};
