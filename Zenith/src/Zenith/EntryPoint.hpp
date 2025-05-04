#pragma once

#include "Zenith/Core/Application.hpp"
#include "Zenith/Core/Assert.hpp"

extern Zenith::Application* Zenith::CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
		Zenith::IntializeCore();
		Zenith::Application* app = Zenith::CreateApplication(argc, argv);
		ZN_CORE_ASSERT(app, "Client Application is null!");
		app->Run();
		delete app;
		Zenith::ShutdownCore();
}