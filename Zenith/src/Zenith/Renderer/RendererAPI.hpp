#pragma once

namespace Zenith {

  using RendererID = uint32_t;

  enum class RendererAPIType
  {
    None = 0,
    OpenGL = 1
  };

  struct RenderAPICapabilities
  {
    std::string Vendor;
    std::string Renderer;
    std::string Version;

    int MaxSamples;
    float MaxAnisotropy;
  };

  class RendererAPI
  {
  public:
    static void Init();
    static void Shutdown();

    static void Clear(float r, float g, float b, float a);
    static void SetClearColor(float r, float g, float b, float a);

    static void DrawIndexed(uint32_t count, bool depthTest = true);

    static RenderAPICapabilities& GetCapabilities()
    {
      static RenderAPICapabilities capabilities;
      return capabilities;
    }

    static RendererAPIType Current() { return s_CurrentRendererAPI; }
  private:
    static RendererAPIType s_CurrentRendererAPI;
  };

}