#include "znpch.hpp"
#include "Window.hpp"

#include "Zenith/Core/Events/ApplicationEvent.hpp"
#include "Zenith/Core/Events/KeyEvent.hpp"
#include "Zenith/Core/Events/MouseEvent.hpp"

namespace Zenith {

	static void GLFWErrorCallback(int error, const char* description)
	{
		ZN_CORE_ERROR("GLFW Erorr ({0}): {1}", error, description);
	}

	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new Window(props);
	}

	Window::Window(const WindowProps& props)
	{
		Init(props);
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		ZN_CORE_INFO("Creating window {0} ({1}x{2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			ZN_CORE_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		ZN_CORE_ASSERT(m_Window, "GLFW window creation failed!");

		glfwMakeContextCurrent(m_Window);
		glfwMaximizeWindow(m_Window);

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
		RegisterGLFWCallbacks();

		int width, height;
		glfwGetWindowSize(m_Window, &width, &height);
		m_Data.Width = width;
		m_Data.Height = height;
	}

	void Window::RegisterGLFWCallbacks()
	{
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event((uint32_t)width, (uint32_t)height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t codepoint)
			{
				auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
				KeyTypedEvent event((int)codepoint);
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
			{
				auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
				MouseMovedEvent event((float)x, (float)y);
				data.EventCallback(event);
			});
	}


	void Window::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void Window::SetVSync(bool enabled)
	{
		glfwSwapInterval(enabled ? 1 : 0);
		m_Data.VSync = enabled;
	}

	std::pair<float, float> Window::GetWindowPos() const
	{
		int x, y;
		glfwGetWindowPos(m_Window, &x, &y);
		return { (float)x, (float)y };
	}

	void Window::Shutdown()
	{
		if (m_Window)
		{
			glfwDestroyWindow(m_Window);
			m_Window = nullptr;
		}
	}

}
