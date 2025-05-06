#include "znpch.hpp"
#include "VertexArray.hpp"

#include "Renderer.hpp"
#include "Zenith/Platform/OpenGL/OpenGLVertexArray.hpp"

namespace Zenith {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:    ZN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPIType::OpenGL:  return std::make_shared<OpenGLVertexArray>();
		}

		ZN_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}