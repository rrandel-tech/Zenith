#include "Zenith.hpp"
#include "Zenith/EntryPoint.hpp"

#include "Zenith/ImGui/ImGuiLayer.hpp"

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

  virtual void OnUpdate() override
  {}

  virtual void OnEvent(Zenith::Event& event) override
  {}
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
