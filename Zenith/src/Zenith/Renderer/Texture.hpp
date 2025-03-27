#pragma once

#include "Zenith/Core/Base.hpp"
#include "RendererAPI.hpp"

namespace Zenith {

  enum class TextureFormat
  {
    None = 0,
    RGB = 1,
    RGBA = 2,
  };

  class Texture
  {
  public:
    virtual ~Texture() {}

    virtual RendererID GetRendererID() const = 0;
  };

  class Texture2D : public Texture
  {
  public:
    static Texture2D* Create(TextureFormat format, uint32_t width, uint32_t height);
    static Texture2D* Create(const std::string& path, bool srgb = false);

    virtual void Bind(uint32_t slot = 0) const = 0;

    virtual TextureFormat GetFormat() const = 0;
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual const std::string& GetPath() const = 0;
  };

  class TextureCube : public Texture
  {
  public:
    static TextureCube* Create(const std::string& path);

    virtual void Bind(uint32_t slot = 0) const = 0;

    virtual TextureFormat GetFormat() const = 0;
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual const std::string& GetPath() const = 0;
  };

}