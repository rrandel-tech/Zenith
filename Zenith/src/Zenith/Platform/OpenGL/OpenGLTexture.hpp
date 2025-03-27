#pragma once

#include "Zenith/Renderer/RendererAPI.hpp"
#include "Zenith/Renderer/Texture.hpp"

namespace Zenith {

  class OpenGLTexture2D : public Texture2D
  {
  public:
    OpenGLTexture2D(TextureFormat format, uint32_t width, uint32_t height);
    OpenGLTexture2D(const std::string& path, bool srgb);
    virtual ~OpenGLTexture2D();

    virtual void Bind(uint32_t slot = 0) const;

    virtual TextureFormat GetFormat() const { return m_Format; }
    virtual uint32_t GetWidth() const { return m_Width; }
    virtual uint32_t GetHeight() const { return m_Height; }

    virtual const std::string& GetPath() const override { return m_FilePath; }

    virtual RendererID GetRendererID() const override { return m_RendererID; }
  private:
    RendererID m_RendererID;
    TextureFormat m_Format;
    uint32_t m_Width, m_Height;

    unsigned char* m_ImageData;

    std::string m_FilePath;
  };

  class OpenGLTextureCube : public TextureCube
  {
  public:
    OpenGLTextureCube(const std::string& path);
    virtual ~OpenGLTextureCube();

    virtual void Bind(uint32_t slot = 0) const;

    virtual TextureFormat GetFormat() const { return m_Format; }
    virtual uint32_t GetWidth() const { return m_Width; }
    virtual uint32_t GetHeight() const { return m_Height; }

    virtual const std::string& GetPath() const override { return m_FilePath; }

    virtual RendererID GetRendererID() const override { return m_RendererID; }
  private:
    RendererID m_RendererID;
    TextureFormat m_Format;
    uint32_t m_Width, m_Height;

    unsigned char* m_ImageData;

    std::string m_FilePath;
  };
}