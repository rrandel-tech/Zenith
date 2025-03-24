#include "znpch.hpp"
#include "Zenith/Renderer/RendererAPI.hpp"

#include <Glad/glad.h>

namespace Zenith {

  void RendererAPI::Clear(float r, float g, float b, float a)
  {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void RendererAPI::SetClearColor(float r, float g, float b, float a)
  {
    glClearColor(r, g, b, a);
  }

}