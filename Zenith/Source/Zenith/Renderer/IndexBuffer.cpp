#include "znpch.hpp"
#include "IndexBuffer.hpp"

#include "Zenith/Platform/OpenGL/OpenGLIndexBuffer.hpp"

namespace Zenith {

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::OpenGL:  return znew OpenGLIndexBuffer(size);
		}
		ZN_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;

	}

}
