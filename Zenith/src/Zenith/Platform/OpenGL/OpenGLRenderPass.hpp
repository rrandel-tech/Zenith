#pragma once

#include "Zenith/Renderer/RenderPass.hpp"

namespace Zenith {

  class OpenGLRenderPass : public RenderPass
  {
  public:
    OpenGLRenderPass(const RenderPassSpecification& spec);
    virtual ~OpenGLRenderPass();

    virtual const RenderPassSpecification& GetSpecification() const override { return m_Specification; }
  private:
    RenderPassSpecification m_Specification;
  };

}