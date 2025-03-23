#include "znpch.hpp"
#include <glad/glad.h>
#include "WindowsWindow.hpp"

#include "Zenith/Core/Events/ApplicationEvent.hpp"
#include "Zenith/Core/Events/KeyEvent.hpp"
#include "Zenith/Core/Events/MouseEvent.hpp"

#include <imgui.h>

namespace Zenith {

  static void GLFWErrorCallback(int error, const char* description)
  {
    ZN_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
  }

  static bool s_GLFWInitialized = false;

  Window* Window::Create(const WindowProps& props)
  {
    return new WindowsWindow(props);
  }

  WindowsWindow::WindowsWindow(const WindowProps& props)
  {
    Init(props);
  }

  WindowsWindow::~WindowsWindow()
  {}

  void WindowsWindow::Init(const WindowProps& props)
  {
    m_Data.Title = props.Title;
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;

    ZN_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

    if (!s_GLFWInitialized)
    {
      // TODO: glfwTerminate on system shutdown
      int success = glfwInit();
      ZN_CORE_ASSERT(success, "Could not intialize GLFW!");
      glfwSetErrorCallback(GLFWErrorCallback);

      s_GLFWInitialized = true;
    }

    m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(m_Window);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    ZN_CORE_ASSERT(status, "Failed to initialize Glad!");
    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);

    SetGLFWCallbacks();
  }

  void WindowsWindow::SetGLFWCallbacks()
  {
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
    {
      auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

      WindowResizeEvent event((uint32_t)width, (uint32_t)height);
      data.EventCallback(event);
      data.Width = width;
      data.Height = height;
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
    {
      auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

      WindowCloseEvent event;
      data.EventCallback(event);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

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

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
    {
      auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

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
      auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

      MouseScrolledEvent event((float)xOffset, (float)yOffset);
      data.EventCallback(event);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
    {
      auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

      MouseMovedEvent event((float)x, (float)y);
      data.EventCallback(event);
    });
  }

  void WindowsWindow::Shutdown()
  {}

  void WindowsWindow::OnUpdate()
  {
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
  }

  void WindowsWindow::SetVSync(bool enabled)
  {
    if (enabled)
      glfwSwapInterval(1);
    else
      glfwSwapInterval(0);

      m_Data.VSync = enabled;
  }

  bool WindowsWindow::IsVSync() const
  {
    return m_Data.VSync;
  }

}