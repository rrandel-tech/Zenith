#pragma once

#include "Zenith/Core/Base.hpp"
#include "Zenith/Core/Window.hpp"

#include "Zenith/Core/Events/ApplicationEvent.hpp"

namespace Zenith {

	struct ApplicationProps {
		std::string Name = "Zenith Engine";
		uint32_t WindowWidth = 1280;
		uint32_t WindowHeight = 720;

		ApplicationProps() = default;
		ApplicationProps(const std::string& name, uint32_t width, uint32_t height)
			: Name(name), WindowWidth(width), WindowHeight(height) {
		}
	};

	class Application {
	public:
		explicit Application(const ApplicationProps& props = {});
		virtual ~Application();

		void Run();

		virtual void OnInit() {}
		virtual void OnShutdown() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event);

		Window& GetWindow() const { return *m_Window; }
		static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

		static Application* s_Instance;
	};

	// Implemented by CLIENT
	[[nodiscard]] Application* CreateApplication(int argc, char** argv);
}