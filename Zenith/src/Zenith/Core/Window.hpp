#pragma once

#include <functional>

#include "Zenith/Core/Base.hpp"
#include "Zenith/Core/Events/Event.hpp"

namespace Zenith {

  struct WindowProps
  {
    std::string Title;
    uint32_t Width;
    uint32_t Height;

    WindowProps(const std::string& title = "Zenith Engine",
      uint32_t width = 1280,
      uint32_t height = 720)
      : Title(title), Width(width), Height(height)
    {
    }
  };

  class Window
  {
  public:
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() {}

    virtual void OnUpdate() = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual std::pair<float, float> GetWindowPos() const = 0;

    // Window attributes
    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    virtual void* GetNativeWindow() const = 0;

    static Window* Create(const WindowProps& props = WindowProps());
  };

}