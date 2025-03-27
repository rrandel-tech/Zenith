#include "znpch.hpp"
#include "OpenGLBuffer.hpp"

#include <Glad/glad.h>

namespace Zenith {

  //////////////////////////////////////////////////////////////////////////////////
  // VertexBuffer
  //////////////////////////////////////////////////////////////////////////////////

  OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
    : m_RendererID(0), m_Size(size)
  {
    Renderer::Submit([=](){
      glGenBuffers(1, &m_RendererID);
    });
  }

  OpenGLVertexBuffer::~OpenGLVertexBuffer()
  {
    Renderer::Submit([=](){
      glDeleteBuffers(1, &m_RendererID);
    });
  }

  void OpenGLVertexBuffer::SetData(void* buffer, uint32_t size, uint32_t offset)
  {
    m_Size = size;
    Renderer::Submit([=](){
      glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
      glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
    });
  }

  void OpenGLVertexBuffer::Bind() const
  {
    Renderer::Submit([=](){
      glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

      // TODO: Extremely temp, by default provide positions and texcoord attributes
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);

      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (const void*)(3 * sizeof(float)));
    });
  }

  //////////////////////////////////////////////////////////////////////////////////
  // IndexBuffer
  //////////////////////////////////////////////////////////////////////////////////

  OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t size)
    : m_RendererID(0), m_Size(size)
  {
    Renderer::Submit([=](){
      glGenBuffers(1, &m_RendererID);
    });
  }

  OpenGLIndexBuffer::~OpenGLIndexBuffer()
  {
    Renderer::Submit([=](){
      glDeleteBuffers(1, &m_RendererID);
    });
  }

  void OpenGLIndexBuffer::SetData(void* buffer, uint32_t size, uint32_t offset)
  {
    m_Size = size;
    Renderer::Submit([=](){
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
    });
  }

  void OpenGLIndexBuffer::Bind() const
  {
    Renderer::Submit([=](){
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    });
  }

}