#pragma once

#include "Zenith/Renderer/VertexBuffer.hpp"


namespace Zenith {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void SetData(void* buffer, uint32_t size, uint32_t offset = 0);
		virtual void Bind() const;

		virtual uint32_t GetSize() const { return m_Size; }
		virtual RendererID GetRendererID() const { return m_RendererID; }
	private:
		RendererID m_RendererID;
		uint32_t m_Size;
	};

}
