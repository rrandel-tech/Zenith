#pragma once

#include "Zenith/Core/Base.hpp"
#include "Zenith/Core/TimeStep.hpp"
#include "Zenith/Core/Window.hpp"
#include "Zenith/Core/LayerStack.hpp"

#include "Zenith/Core/Events/ApplicationEvent.hpp"

#include "Zenith/ImGui/ImGuiLayer.hpp"

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
		virtual void OnUpdate(Timestep ts) {}

		virtual void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);
		void RenderImGui();

		std::string OpenFile(const char* filter = "All\0*.*\0") const;
		std::string SaveFile(const char* filter = "All\0*.*\0") const;

		Window& GetWindow() const { return *m_Window; }

		static Application& Get() { return *s_Instance; }

		float GetTime() const; // TODO: This should be in "Platform"

		static const char* GetConfigurationName();
		static const char* GetPlatformName();
	private:
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true, m_Minimized = false;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		Timestep m_TimeStep;

		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;
	};

	// Implemented by CLIENT
	[[nodiscard]] Application* CreateApplication(int argc, char** argv);
}