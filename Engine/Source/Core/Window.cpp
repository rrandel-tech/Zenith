#include "znpch.hpp"
#include "Window.hpp"

namespace Zenith {

    std::unique_ptr<Window> Window::Create(const WindowSpecification& specification)
    {
        return std::make_unique<Window>(specification);
    }

    Window::Window(const WindowSpecification& props)
        : m_Specification(props), m_Data()
    {
    }

    Window::~Window()
    {
        Window::shutdown();
    }

    void Window::Init()
    {
        m_Data.Title = m_Specification.Title;
        m_Data.Width = m_Specification.Width;
        m_Data.Height = m_Specification.Height;

        ZN_CORE_INFO("Creating window {0} ({1}, {2})", m_Specification.Title, m_Specification.Width, m_Specification.Height);
    }

    void Window::shutdown()
    {
    }

    void Window::ProcessEvents()
    {
    }

    void Window::SwapBuffers()
    {
        // swapchain
    }

    void Window::SetTitle(const std::string& title)
    {
        m_Data.Title = title;
    }

}