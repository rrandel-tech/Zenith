#pragma once

#include "Base.hpp"
#include "TimeStep.hpp"
#include "Window.hpp"
#include "LayerStack.hpp"

#include "Events/ApplicationEvent.hpp"

#include <string>
#include <memory>
#include <deque>

namespace Zenith {

    struct ApplicationSpecification
    {
        std::string Name = "Zenith";
        uint32_t WindowWidth = 1600, WindowHeight = 900;
        WindowMode Mode = WindowMode::Windowed;
        bool VSync = true;
        bool Resizable = true;
        std::string WorkingDirectory;
        std::filesystem::path IconPath;
    };

    class Application
    {
        using EventCallbackFn = std::function<void(Event&)>;
    public:
        Application(const ApplicationSpecification& specification);
        virtual ~Application();

        void Run();
        void Close();

        virtual void OnInit() {}
        virtual void OnShutdown();
        virtual void OnUpdate(Timestep ts) {}

        virtual void OnEvent(Event& event);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* layer);

        void AddEventCallback(const EventCallbackFn& eventCallback) { m_EventCallbacks.push_back(eventCallback); }

        template<typename Func>
        void QueueEvent(Func&& func)
        {
            std::scoped_lock<std::mutex> lock(m_EventQueueMutex);
            m_EventQueue.emplace_back(true, func);
        }

        // Creates & Dispatches an event either immediately, or adds it to an event queue which will be processed after the next call
        // to SyncEvents().
        // Waiting until after next sync gives the application some control over _when_ the events will be processed.
        // An example of where this is useful:
        // Suppose an asset thread is loading assets and dispatching "AssetReloaded" events.
        // We do not want those events to be processed until the asset thread has synced its assets back to the main thread.
        template<typename TEvent, bool DispatchImmediately = false, typename... TEventArgs>
        void DispatchEvent(TEventArgs&&... args)
        {
#ifndef ZN_COMPILER_GCC
            static_assert(std::is_assignable_v<Event, TEvent>);
#endif

            std::shared_ptr<TEvent> event = std::make_shared<TEvent>(std::forward<TEventArgs>(args)...);
            if constexpr (DispatchImmediately)
            {
                OnEvent(*event);
            }
            else
            {
                std::scoped_lock<std::mutex> lock(m_EventQueueMutex);
                m_EventQueue.emplace_back(false, [event](){ Application::Get().OnEvent(*event); });
            }
        }

        // Mark all waiting events as sync'd.
        // Thus allowing them to be processed on next call to ProcessEvents()
        void SyncEvents();

        inline Window& GetWindow() { return *m_Window; }

        static inline Application& Get() { return *s_Instance; }

        Timestep GetTimestep() const { return m_TimeStep; }
        Timestep GetFrametime() const { return m_Frametime; }
        float GetTime() const; // TODO: This should be in "Platform"

        static const char* GetConfigurationName();
        static const char* GetPlatformName();

        const ApplicationSpecification& GetSpecification() const { return m_Specification; }
    private:
        void ProcessEvents();

        bool OnWindowResize(WindowResizeEvent& e);
        bool OnWindowMinimize(WindowMinimizeEvent& e);
        bool OnWindowClose(WindowCloseEvent& e);
    private:
        std::unique_ptr<Window> m_Window;
        ApplicationSpecification m_Specification;
        bool m_Running = true, m_Minimized = false;
        LayerStack m_LayerStack;
        Timestep m_Frametime;
        Timestep m_TimeStep;

        float m_LastFrameTime = 0.0f;

        std::mutex m_EventQueueMutex;
        std::deque<std::pair<bool, std::function<void()>>> m_EventQueue;
        std::vector<EventCallbackFn> m_EventCallbacks;

        static Application* s_Instance;
    };

    // Implemented by CLIENT
    Application* CreateApplication(int argc, char** argv);
}