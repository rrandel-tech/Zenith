#include "znpch.hpp"
#include "ImGuiLayer.hpp"

#include "Zenith/Renderer/Renderer.hpp"

#include "Zenith/Platform/OpenGL/OpenGLImGuiLayer.hpp"

#include "Zenith/Renderer/RendererAPI.hpp"

#include <imgui.h>

namespace Zenith {

	ImGuiLayer* ImGuiLayer::Create()
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::OpenGL:  return new OpenGLImGuiLayer();
		}
		ZN_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}
