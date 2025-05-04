#pragma once

#include "Zenith/Core/Base.hpp"

namespace Zenith {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:

	};

	// Implemented by CLIENT
	[[nodiscard]] Application* CreateApplication(int argc, char** argv);
}