#pragma once

#include "Zenith/Core/Base.hpp"
#include "Zenith/Core/Events/Event.hpp"

#include <functional>

#include <GLFW/glfw3.h>

namespace Zenith {

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Zenith Engine",
			uint32_t width = 1280,
			uint32_t height = 720)
			: Title(title), Width(width), Height(height)
		{}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		static Window* Create(const WindowProps& props = WindowProps());

		explicit Window(const WindowProps& props);
		~Window();

		void Init(const WindowProps& props);
		void OnUpdate();
		void RegisterGLFWCallbacks();
		void Shutdown();

		[[nodiscard]] uint32_t GetWidth() const { return m_Data.Width; }
		[[nodiscard]] uint32_t GetHeight() const { return m_Data.Height; }
		[[nodiscard]] std::pair<uint32_t, uint32_t> GetSize() const { return { m_Data.Width, m_Data.Height }; }
		[[nodiscard]] std::pair<float, float> GetWindowPos() const;
		[[nodiscard]] bool IsVSync() const { return m_Data.VSync; }
		[[nodiscard]] void* GetNativeWindow() const { return m_Window; }

		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled);

	private:
		struct WindowData
		{
			std::string Title;
			uint32_t Width = 0, Height = 0;
			bool VSync = true;
			EventCallbackFn EventCallback;
		};

		GLFWwindow* m_Window = nullptr;
		WindowData m_Data;
		float m_LastFrameTime = 0.0f;
	};

}
