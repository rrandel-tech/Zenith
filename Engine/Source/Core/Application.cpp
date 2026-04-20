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

    void Application::SyncEvents()
    {
        std::scoped_lock<std::mutex> lock(m_EventQueueMutex);
        for (auto& [synced, _] : m_EventQueue)
        {
            synced = true;
        }
    }

    void Application::Run()
    {
        OnInit();
        m_LastFrameTime = GetTime();
        while (m_Running)
        {
            float time = GetTime();
            m_Frametime = time - m_LastFrameTime;
            m_TimeStep = glm::min<float>(m_Frametime, 0.333f);
            m_LastFrameTime = time;

            ProcessEvents();

            if (!m_Minimized)
            {
                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate(m_TimeStep);

                m_Window->SwapBuffers();
            }

            Input::ClearReleasedKeys();

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

        // Note: we have no control over what func() does.  holding this lock while calling func() is a bad idea:
        // 1) func() might be slow (means we hold the lock for ages)
        // 2) func() might result in events getting queued, in which case we have a deadlock
        std::scoped_lock<std::mutex> lock(m_EventQueueMutex);

        // Process custom event queue, up until we encounter an event that is not yet sync'd
        // If application queues such events, then it is the application's responsibility to call
        // SyncEvents() at the appropriate time.
        while (m_EventQueue.size() > 0)
        {
            const auto& [synced, func] = m_EventQueue.front();
            if (!synced)
            {
                break;
            }
            func();
            m_EventQueue.pop_front();
        }
    }

    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return OnWindowResize(e); });
        dispatcher.Dispatch<WindowMinimizeEvent>([this](WindowMinimizeEvent& e) { return OnWindowMinimize(e); });
        dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return OnWindowClose(e); });

        //ZN_CORE_TRACE("{0}", event.ToString());

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
        {
            (*--it)->OnEvent(event);
            if (event.Handled)
                break;
        }

        if (event.Handled)
            return;

        // TODO: Should these callbacks be called BEFORE the layers recieve events?
        //				We may actually want that since most of these callbacks will be functions REQUIRED in order for the game
        //				to work, and if a layer has already handled the event we may end up with problems
        for (auto& eventCallback : m_EventCallbacks)
        {
            eventCallback(event);

            if (event.Handled)
                break;
        }
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

    float Application::GetTime() const
    {
        static const double frequency = (double)SDL_GetPerformanceFrequency();
        static const double start = (double)SDL_GetPerformanceCounter() / frequency;
        return (float)((double)SDL_GetPerformanceCounter() / frequency - start);
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
