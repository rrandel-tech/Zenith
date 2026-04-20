#pragma once

#include "Base.hpp"
#include "Events/Event.hpp"

#include <SDL3/SDL.h>

#include <functional>
#include <filesystem>

namespace Zenith {

    enum class WindowMode
    {
        Windowed = 0,
        BorderlessFullscreen,
        ExclusiveFullscreen,
        Maximized
    };

    struct WindowSpecification
    {
        std::string Title = "Zenith";
        uint32_t Width = 1600, Height = 900;
        WindowMode Mode = WindowMode::Windowed;
        bool VSync = true;
        std::filesystem::path IconPath;
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

        virtual std::pair<uint32_t, uint32_t> GetSize() const { return { m_Data.Width, m_Data.Height }; }
        virtual std::pair<float, float> GetWindowPos() const;

        // Window attributes
        void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled);
        bool IsVSync() const;
        void SetResizable(bool resizable) const;

        void Maximize();
        void CenterWindow();

        const std::string& GetTitle() const { return m_Data.Title; }
        void SetTitle(const std::string& title);

        inline void* GetNativeWindow() const { return m_Window; }

        static Window* Create(const WindowSpecification& specification = WindowSpecification());
    private:
        void PollEvents();
        void Shutdown();
    private:
        SDL_Window* m_Window = nullptr;
        SDL_Event m_Event {};

        WindowSpecification m_Specification;
        struct WindowData
        {
            std::string Title;
            uint32_t Width, Height;

            EventCallbackFn EventCallback;
        }; WindowData m_Data;
    };

}