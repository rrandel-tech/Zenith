#include "znpch.hpp"
#include "OpenGLIndexBuffer.hpp"

#include <glad/glad.h>

#include "Zenith/Renderer/Renderer.hpp"

namespace Zenith {

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
