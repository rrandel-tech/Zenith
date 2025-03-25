#include "znpch.hpp"
#include "Shader.hpp"

#include "Zenith/Platform/OpenGL/OpenGLShader.hpp"

namespace Zenith {

  Shader* Shader::Create(const std::string& filepath)
  {
    switch (RendererAPI::Current())
    {
    case RendererAPIType::None: return nullptr;
    case RendererAPIType::OpenGL: return new OpenGLShader(filepath);
    }
    return nullptr;
  }

}