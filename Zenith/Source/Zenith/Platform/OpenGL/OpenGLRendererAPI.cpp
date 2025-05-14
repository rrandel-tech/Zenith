#include "znpch.hpp"
#include "Zenith/Renderer/RendererAPI.hpp"

#include <Glad/glad.h>

namespace Zenith {

	static void OpenGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:
				ZN_CORE_ERROR_TAG("Renderer", "[OpenGL Debug HIGH] {0}", message);
				ZN_CORE_ASSERT(false, "GL_DEBUG_SEVERITY_HIGH");
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				ZN_CORE_ERROR_TAG("Renderer", "[OpenGL Debug MEDIUM] {0}", message);
				break;
			case GL_DEBUG_SEVERITY_LOW:
				ZN_CORE_ERROR_TAG("Renderer", "[OpenGL Debug LOW] {0}", message);
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				ZN_CORE_ERROR_TAG("Renderer", "[OpenGL Debug NOTIFICATION] {0}", message);
				break;
		}
	}

	void RendererAPI::Init()
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLLogMessage, nullptr);

		uint32_t vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		auto& caps = RendererAPI::GetCapabilities();

		caps.Vendor = (const char*)glGetString(GL_VENDOR);
		caps.Renderer = (const char*)glGetString(GL_RENDERER);
		caps.Version = (const char*)glGetString(GL_VERSION);

		glGetIntegerv(GL_MAX_SAMPLES, &caps.MaxSamples);
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &caps.MaxAnisotropy);

		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &caps.MaxTextureUnits);

		for (GLenum error = glGetError(); error != GL_NO_ERROR; error = glGetError())
			ZN_CORE_ERROR_TAG("Renderer", "OpenGL Error on Init: 0x{:X}", error);
	}

	void RendererAPI::Shutdown()
	{}

	void RendererAPI::Clear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererAPI::DrawIndexed(uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr);
	}

}
