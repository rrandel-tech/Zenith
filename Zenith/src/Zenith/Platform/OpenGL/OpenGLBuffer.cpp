
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
		Renderer::Submit([=]() {
			glGenBuffers(1, &m_RendererID);
		});
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		Renderer::Submit([=]() {
			glDeleteBuffers(1, &m_RendererID);
		});
	}

	void OpenGLVertexBuffer::SetData(void* buffer, uint32_t size, uint32_t offset)
	{
		Renderer::Submit([=]() {
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
			glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
			});

	}

	void OpenGLVertexBuffer::Bind() const
	{
		Renderer::Submit([=]() {
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		});
	}

	//////////////////////////////////////////////////////////////////////////////////
	// IndexBuffer
	//////////////////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t size)
		: m_RendererID(0), m_Size(size)
	{
		Renderer::Submit([=]() {
			glGenBuffers(1, &m_RendererID);
		});
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		Renderer::Submit([=]() {
			glDeleteBuffers(1, &m_RendererID);
		});
	}

	void OpenGLIndexBuffer::SetData(void* buffer, uint32_t size, uint32_t offset)
	{
		Renderer::Submit([=]() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
		});
	}

	void OpenGLIndexBuffer::Bind() const
	{
		Renderer::Submit([=]() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		});
	}

}
