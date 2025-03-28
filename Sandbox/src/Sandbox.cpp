#include "Zenith.hpp"
#include "Zenith/EntryPoint.hpp"

#include "Zenith/ImGui/ImGuiLayer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include <string>

class TestLayer : public Zenith::Layer
{
public:
  TestLayer()
  {}

  virtual ~TestLayer()
  {}

  virtual void OnAttach() override
  {}

  virtual void OnDetach() override
  {}

  virtual void OnUpdate(Zenith::Timestep ts) override
  {
    Zenith::Renderer::ClearMagenta();
  }

  virtual void OnImGuiRender() override
  {}

  virtual void OnEvent(Zenith::Event& event) override
  {}
private:
};

class Sandbox : public Zenith::Application
{
public:
  Sandbox()
  {
    ZN_TRACE("Hello!");
  }

  virtual void OnInit() override
  {
    PushLayer(new TestLayer());
  }
};

Zenith::Application* Zenith::CreateApplication()
{
  return new Sandbox();
}
