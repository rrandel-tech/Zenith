#pragma once

#include "znpch.hpp"
#include "Zenith/Core/Layer.hpp"

namespace Zenith {

  class ImGuiLayer : public Layer
  {
  public:
    ImGuiLayer();
    ImGuiLayer(const std::string& name);
    virtual ~ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;
  private:
    float m_Time = 0.0f;
  };

}