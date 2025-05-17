#include "znpch.hpp"
#include "RendererContext.hpp"

#include "Zenith/Renderer/RendererAPI.hpp"

#include "Zenith/Platform/OpenGL/OpenGLContext.hpp"

namespace Zenith {

	Ref<RendererContext> RendererContext::Create(GLFWwindow* windowHandle)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::OpenGL:  return Ref<OpenGLContext>::Create(windowHandle);
		}
		ZN_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}
