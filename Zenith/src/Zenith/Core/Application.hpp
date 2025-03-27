#pragma once

#include "Zenith/Core/Base.hpp"
#include "Zenith/Core/Window.hpp"
#include "Zenith/Core/LayerStack.hpp"

#include "Zenith/Core/Events/ApplicationEvent.hpp"

#include "Zenith/ImGui/ImGuiLayer.hpp"

namespace Zenith {

  class Application
  {
  public:
    Application();
    virtual ~Application();

    void Run();

    virtual void OnInit() {}
    virtual void OnShutdown() {}
    virtual void OnUpdate() {}

    virtual void OnEvent(Event& event);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);
    void RenderImGui();

    std::string OpenFile(const std::string& filter) const;

    inline Window& GetWindow() { return *m_Window; }

    static inline Application& Get() { return *s_Instance; }
  private:
    bool OnWindowResize(WindowResizeEvent& e);
    bool OnWindowClose(WindowCloseEvent& e);
  private:
    std::unique_ptr<Window> m_Window;
    bool m_Running = true, m_Minimized = false;
    LayerStack m_LayerStack;
    ImGuiLayer* m_ImGuiLayer;

    static Application* s_Instance;
  };

  // Implemented by CLIENT
  Application* CreateApplication();
};
