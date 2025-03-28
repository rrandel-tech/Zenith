#pragma once

#include "Zenith/Core/Window.hpp"

#include <GLFW/glfw3.h>

namespace Zenith {

  class WindowsWindow : public Window
  {
  public:
    WindowsWindow(const WindowProps& props);
    virtual ~WindowsWindow();

    void OnUpdate() override;

    inline uint32_t GetWidth() const override { return m_Data.Width; }
    inline uint32_t GetHeight() const override { return m_Data.Height; }

    virtual std::pair<float, float> GetWindowPos() const override;

    // Window attributes
    inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
    void SetVSync(bool enabled);
    bool IsVSync() const;

    inline void* GetNativeWindow() const { return m_Window; }
  private:
    virtual void Init(const WindowProps& props);
    virtual void Shutdown();
    virtual void SetGLFWCallbacks();
  private:
    GLFWwindow* m_Window;

    struct WindowData
    {
      std::string Title;
      uint32_t Width, Height;
      bool VSync;

      EventCallbackFn EventCallback;
    };

    WindowData m_Data;
    float m_LastFrameTime = 0.0f;
  };

}