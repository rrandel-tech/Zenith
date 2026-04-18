#pragma once

#include "Events/Event.hpp"

#include <functional>
#include <memory>

namespace Zenith {

    struct WindowSpecification
    {
        std::string Title = "Zenith";
        uint32_t Width = 1600, Height = 900;
    };

    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        Window(const WindowSpecification& specification);
        ~Window();

        void Init();
        void ProcessEvents();
        void SwapBuffers();

        uint32_t GetWidth() const { return m_Data.Width; }
        uint32_t GetHeight() const { return m_Data.Height; }

        // Window attributes
        void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

        const std::string& GetTitle() const { return m_Data.Title; }
        void SetTitle(const std::string& title);

        static std::unique_ptr<Window> Create(const WindowSpecification& specification = WindowSpecification());
    private:
        void shutdown();
    private:
        WindowSpecification m_Specification;
        struct WindowData
        {
            std::string Title;
            uint32_t Width, Height;

            EventCallbackFn EventCallback;
        }; WindowData m_Data;
    };

}