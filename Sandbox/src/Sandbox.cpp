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

Zenith::Application* Zenith::CreateApplication(int argc, char** argv)
{
	return new Sandbox();
}