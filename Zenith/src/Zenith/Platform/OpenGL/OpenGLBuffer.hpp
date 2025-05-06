#pragma once

#include "Zenith/Renderer/Buffer.hpp"

namespace Zenith {

	//////////////////////////////////////////////////////////////////////////////////
	// VertexBuffer
	//////////////////////////////////////////////////////////////////////////////////

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

	//////////////////////////////////////////////////////////////////////////////////
	// IndexBuffer
	//////////////////////////////////////////////////////////////////////////////////

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t size);
		virtual ~OpenGLIndexBuffer();

		virtual void SetData(void* buffer, uint32_t size, uint32_t offset = 0);
		virtual void Bind() const;

		virtual uint32_t GetCount() const { return m_Size / sizeof(uint32_t); }

		virtual uint32_t GetSize() const { return m_Size; }
		virtual RendererID GetRendererID() const { return m_RendererID; }
	private:
		RendererID m_RendererID;
		uint32_t m_Size;
	};

}