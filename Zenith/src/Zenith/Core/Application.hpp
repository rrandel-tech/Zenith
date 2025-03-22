#pragma once

#include "Zenith/Core/Base.hpp"
#include "Zenith/Core/Window.hpp"

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
  };

  // Implemented by CLIENT
  Application* CreateApplication();
};
