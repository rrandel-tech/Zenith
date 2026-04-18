#pragma once

#include "Core/Application.hpp"
#include "Core/Assert.hpp"

int main(int argc, char** argv)
{
    Zenith::InitializeCore();
    Zenith::Application* app = Zenith::CreateApplication(argc, argv);
    ZN_CORE_ASSERT(app, "Client Application is null!");
    app->Run();
    delete app;
    Zenith::ShutdownCore();
}