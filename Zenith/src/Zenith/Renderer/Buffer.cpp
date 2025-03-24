#include "znpch.hpp"

#include "Zenith/Platform/OpenGL/OpenGLBuffer.hpp"

namespace Zenith {

  VertexBuffer* VertexBuffer::Create(uint32_t size)
  {
    switch (RendererAPI::Current())
    {
    case RendererAPIType::None:    return nullptr;
    case RendererAPIType::OpenGL:  return new OpenGLVertexBuffer(size);
    }
    return nullptr;

  }

  IndexBuffer* IndexBuffer::Create(uint32_t size)
  {
    switch (RendererAPI::Current())
    {
    case RendererAPIType::None:    return nullptr;
    case RendererAPIType::OpenGL:  return new OpenGLIndexBuffer(size);
    }
    return nullptr;

  }

}