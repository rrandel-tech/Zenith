#pragma once

#include "Core/Base.hpp"
#include "Window.hpp"
#include "Core/LayerStack.hpp"

#include "Events/ApplicationEvent.hpp"

#include <string>
#include <memory>

namespace Zenith {

    struct ApplicationSpecification
    {
        std::string Name = "Zenith";
        uint32_t WindowWidth = 1600, WindowHeight = 900;
        WindowMode Mode = WindowMode::Windowed;
        bool VSync = true;
        bool Resizable = true;
    };

    class Application
    {
        using EventCallbackFn = std::function<void(Event&)>;
    public:
        explicit Application(const ApplicationSpecification& specification);
        virtual ~Application();

        void Run();
        void Close();

        virtual void OnInit() {}

        void OnShutdown();

        void OnUpdate() {}
        void OnEvent(Event& event);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* layer);

        void AddEventCallback(const EventCallbackFn& eventCallback) { m_EventCallbacks.push_back(eventCallback); }

        static const char* GetConfigurationName();
        static const char* GetPlatformName();

        [[nodiscard]] const ApplicationSpecification& GetSpecification() const { return m_Specification; }
    private:
        void processEvents();

        bool OnWindowResize(WindowResizeEvent& e);
        bool OnWindowMinimize(WindowMinimizeEvent& e);
        bool OnWindowClose(WindowCloseEvent& e);
    private:
        std::unique_ptr<Window> m_Window;
        ApplicationSpecification m_Specification;
        bool m_Running = true, m_Minimized = false;
        LayerStack m_LayerStack;

        std::vector<EventCallbackFn> m_EventCallbacks;
    };

    // Implemented by CLIENT
    Application* CreateApplication(int argc, char** argv);
}