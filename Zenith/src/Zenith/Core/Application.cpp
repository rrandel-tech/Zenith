#include "znpch.hpp"
#include "Application.hpp"

#include "Zenith/Renderer/Renderer.hpp"
#include <GLFW/glfw3.h>

namespace Zenith {

  #define BIND_EVENT_FN(fn) std::bind(&Application::##fn, this, std::placeholders::_1)

  Application* Application::s_Instance = nullptr;

  Application::Application()
  {
    s_Instance = this;

    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

    m_ImGuiLayer = new ImGuiLayer("ImGui");
    PushOverlay(m_ImGuiLayer);
  }

  Application::~Application()
  {}

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

  void Application::RenderImGui()
  {
    m_ImGuiLayer->Begin();
    for (Layer* layer : m_LayerStack)
      layer->OnImGuiRender();

    m_ImGuiLayer->End();
  }

  void Application::Run()
  {
    OnInit();
    while (m_Running)
    {
      for (Layer* layer : m_LayerStack)
        layer->OnUpdate();

      // Render ImGui on render thread
      Application* app = this;
      ZN_RENDER_1(app, { app->RenderImGui(); });

      Renderer::Get().WaitAndRender();

      m_Window->OnUpdate();
    }
    OnShutdown();
  }

  void Application::OnEvent(Event& event)
  {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
    {
      (*--it)->OnEvent(event);
      if (event.Handled)
        break;
    }
  }

  bool Application::OnWindowResize(WindowResizeEvent& e)
  {
    return false;
  }

  bool Application::OnWindowClose(WindowCloseEvent& e)
  {
    m_Running = false;
    return true;
  }
}