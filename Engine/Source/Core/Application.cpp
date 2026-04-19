#include "znpch.hpp"
#include "Application.hpp"

#include "Input.hpp"

#include <glm/glm.hpp>

#include <filesystem>

extern bool g_ApplicationRunning;
namespace Zenith {

    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecification& specification)
        : m_Specification(specification)
    {
        s_Instance = this;

        if (!specification.WorkingDirectory.empty())
            std::filesystem::current_path(specification.WorkingDirectory);

        WindowSpecification windowSpec;
        windowSpec.Title = m_Specification.Name;
        windowSpec.Width = m_Specification.WindowWidth;
        windowSpec.Height = m_Specification.WindowHeight;
        windowSpec.Mode = m_Specification.Mode;
        windowSpec.VSync = specification.VSync;
        windowSpec.IconPath = specification.IconPath;
        m_Window = std::unique_ptr<Window>(Window::Create(windowSpec));
        m_Window->Init();
        m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });
        m_Window->SetResizable(m_Specification.Resizable);

        if (m_Specification.Mode == WindowMode::Windowed)
            m_Window->CenterWindow();
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
            ProcessEvents();

            if (!m_Minimized)
            {
                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate(m_TimeStep);

                m_Window->SwapBuffers();
            }

            Input::ClearReleasedKeys();

            float time = GetTime();
            m_Frametime = time - m_LastFrameTime;
            m_TimeStep = glm::min<float>(m_Frametime, 0.333f);
            m_LastFrameTime = time;
            //ZN_CORE_INFO("Frametime: {:.4f}ms | Timestep: {:.4f}ms | FPS: {:.1f}", m_Frametime * 1000.0f, m_TimeStep  * 1000.0f, 1.0f / m_Frametime);
        }
        OnShutdown();
    }

    void Application::Close()
    {
        m_Running = false;
    }

    void Application::OnShutdown()
    {
        m_EventCallbacks.clear();
        g_ApplicationRunning = false;
    }

    void Application::ProcessEvents()
    {
        Input::TransitionPressedKeys();
        Input::TransitionPressedButtons();

        m_Window->ProcessEvents();
    }

    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return OnWindowResize(e); });
        dispatcher.Dispatch<WindowMinimizeEvent>([this](WindowMinimizeEvent& e) { return OnWindowMinimize(e); });
        dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return OnWindowClose(e); });

        //ZN_CORE_TRACE("{0}", event.ToString());
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

    float Application::GetTime() const
    {
        // Returns elapsed seconds since engine start as an absolute timestamp
        static const uint64_t s_StartTime = SDL_GetTicksNS();
        return static_cast<float>((SDL_GetTicksNS() - s_StartTime) * 1e-9);
    }
}
