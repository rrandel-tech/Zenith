#include "znpch.hpp"
#include "Texture.hpp"

#include "Zenith/Renderer/RendererAPI.hpp"
#include "Zenith/Platform/OpenGL/OpenGLTexture.hpp"

namespace Zenith {

	Texture2D* Texture2D::Create(TextureFormat format, uint32_t width, uint32_t height, TextureWrap wrap)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None: return nullptr;
		case RendererAPIType::OpenGL: return new OpenGLTexture2D(format, width, height, wrap);
		}
		return nullptr;
	}

	Texture2D* Texture2D::Create(const std::string& path, bool srgb)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None: return nullptr;
		case RendererAPIType::OpenGL: return new OpenGLTexture2D(path, srgb);
		}
		return nullptr;
	}

	TextureCube* TextureCube::Create(const std::string& path)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None: return nullptr;
		case RendererAPIType::OpenGL: return new OpenGLTextureCube(path);
		}
		return nullptr;
	}

	uint32_t Texture::GetBPP(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RGB:    return 3;
		case TextureFormat::RGBA:   return 4;
		}
		return 0;
	}

}