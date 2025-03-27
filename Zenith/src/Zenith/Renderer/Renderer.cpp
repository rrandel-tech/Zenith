#include "znpch.hpp"
#include "Renderer.hpp"

namespace Zenith {

  Renderer* Renderer::s_Instance = new Renderer();
  RendererAPIType RendererAPI::s_CurrentRendererAPI = RendererAPIType::OpenGL;

  void Renderer::Init()
  {
    Renderer::Submit([](){
      RendererAPI::Init();
    });
  }

  void Renderer::Clear()
  {
    Renderer::Submit([]() {
      RendererAPI::Clear(0.0f, 0.0f, 0.0f, 1.0f);
    });
  }

  void Renderer::Clear(float r, float g, float b, float a)
  {
    Renderer::Submit([=]() {
      RendererAPI::Clear(r, g, b, a);
    });
  }

  void Renderer::ClearMagenta()
  {
    Clear(1, 0, 1);
  }

  void Renderer::SetClearColor(float r, float g, float b, float a)
  {}

  void Renderer::DrawIndexed(uint32_t count, bool depthTest)
  {
    Renderer::Submit([=]() {
      RendererAPI::DrawIndexed(count, depthTest);
    });
  }

  void Renderer::WaitAndRender()
  {
    s_Instance->m_CommandQueue.Execute();
  }

}