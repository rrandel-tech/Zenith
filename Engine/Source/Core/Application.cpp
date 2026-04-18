#include "znpch.hpp"
#include "Application.hpp"

namespace Zenith {

    Application::Application(const ApplicationSpecification& specification)
        : m_specification(specification)
    {
    }

    Application::~Application()
    {
    }

    void Application::Close()
    {
        m_isRunning = false;
    }

    void Application::OnShutdown()
    {
    }

    void Application::Run()
    {
        OnInit();
        while (m_isRunning)
        {
            if (!m_isMinimized)
            {
            }
        }
        OnShutdown();
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