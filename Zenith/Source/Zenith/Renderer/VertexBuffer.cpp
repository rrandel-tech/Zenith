#include "znpch.hpp"

#include "Zenith/Platform/OpenGL/OpenGLVertexBuffer.hpp"

namespace Zenith {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::OpenGL:  return new OpenGLVertexBuffer(size);
		}
		return nullptr;

	}

}
