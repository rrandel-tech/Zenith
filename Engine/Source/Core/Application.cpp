#include "znpch.hpp"
#include "Application.hpp"

namespace Zenith {
    Application::Application(const ApplicationSpecification& specification)
        : m_Specification(specification)
    {
        WindowSpecification windowSpec;
        windowSpec.Title = m_Specification.Name;
        windowSpec.Width = m_Specification.WindowWidth;
        windowSpec.Height = m_Specification.WindowHeight;
        m_Window = std::unique_ptr(Window::Create(windowSpec));
        m_Window->Init();
        m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });
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
            if (!m_Minimized)
            {
                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate();

                m_Window->SwapBuffers();
            }
        }
        OnShutdown();
    }

    void Application::Close()
    {
        m_Running = false;
    }

    void Application::OnShutdown()
    {
    }

    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return OnWindowResize(e); });
        dispatcher.Dispatch<WindowMinimizeEvent>([this](WindowMinimizeEvent& e) { return OnWindowMinimize(e); });
        dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return OnWindowClose(e); });

        ZN_CORE_INFO("{}", event.ToString());
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        return false;
    }

    bool Application::OnWindowMinimize(WindowMinimizeEvent& e)
    {
        m_Minimized = e.IsMinimized();
        return false;
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        Close();
        return false; // give other things a chance to react to window close
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
