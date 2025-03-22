#include "Zenith.hpp"
#include "Zenith/EntryPoint.hpp"

class Sandbox : public Zenith::Application
{
public:
  Sandbox()
  {
    ZN_TRACE("Hello!");
  }
};

Zenith::Application* Zenith::CreateApplication()
{
  return new Sandbox();
}
