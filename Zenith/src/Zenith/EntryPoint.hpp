#pragma once

#include "Zenith/Core/Application.hpp"
#include "Zenith/Core/Assert.hpp"

extern Zenith::Application* Zenith::CreateApplication();

  int main(int argc, char** argv)
  {

      Zenith::InitializeCore();
      Zenith::Application* app = Zenith::CreateApplication();
      ZN_CORE_ASSERT(app, "Client Application is null!");
      app->Run();
      delete app;
      Zenith::ShutdownCore();
  }
