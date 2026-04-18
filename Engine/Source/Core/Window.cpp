#include "znpch.hpp"
#include "Window.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"

namespace Zenith {

    static void SDLErrorCallback(const char* context)
    {
        ZN_CORE_ERROR("SDL Error ({}): {}", context, SDL_GetError());
    }

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
        Window::Shutdown();
    }

    void Window::Init()
    {
        m_Data.Title = m_Specification.Title;
        m_Data.Width = m_Specification.Width;
        m_Data.Height = m_Specification.Height;

        ZN_CORE_INFO("Creating window {0} ({1}, {2})", m_Specification.Title, m_Specification.Width, m_Specification.Height);

        SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

		Uint32 windowFlags = SDL_WINDOW_HIGH_PIXEL_DENSITY;

		switch (m_Specification.Mode)
		{
			case WindowMode::Windowed:
				m_Window = SDL_CreateWindow(m_Data.Title.c_str(), m_Data.Width, m_Data.Height, windowFlags);
				break;
			case WindowMode::BorderlessFullscreen:
				windowFlags |= SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN;
				m_Window = SDL_CreateWindow(m_Data.Title.c_str(), 0, 0, windowFlags);
				break;
			case WindowMode::ExclusiveFullscreen:
			{
				SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
				if (displayID == 0)
				{
					SDLErrorCallback("SDL_GetPrimaryDisplay");
					ZN_CORE_ASSERT(false, "Could not get primary display!");
				}

				const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(displayID);
				if (!mode)
				{
					SDLErrorCallback("SDL_GetCurrentDisplayMode");
					ZN_CORE_ASSERT(false, "Could not get display mode!");
				}

				m_Window = SDL_CreateWindow(m_Data.Title.c_str(), mode->w, mode->h, windowFlags | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_MOUSE_GRABBED);
				break;
			}
		}

		if (!m_Window)
		{
			SDLErrorCallback("SDL_CreateWindow");
			ZN_CORE_ASSERT(false, "Could not create window!");
		}

		// Update window size to actual size
		int w, h;
		SDL_GetWindowSize(m_Window, &w, &h);
		m_Data.Width = w;
		m_Data.Height = h;
	}

	void Window::PollEvents()
	{
		SDL_WindowID windowID = SDL_GetWindowID(m_Window);
		if (windowID == 0)
		{
			SDLErrorCallback("SDL_GetWindowID");
			return;
		}

		while (SDL_PollEvent(&m_Event))
		{
			switch (m_Event.type)
			{
				case SDL_EVENT_QUIT:
				{
					WindowCloseEvent event;
					m_Data.EventCallback(event);
					break;
				}
				case SDL_EVENT_WINDOW_RESIZED:
				{
					if (m_Event.window.windowID == windowID)
					{
						int width, height;
						SDL_GetWindowSize(m_Window, &width, &height);

						WindowResizeEvent event(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
						m_Data.EventCallback(event);
						m_Data.Width = width;
						m_Data.Height = height;
					}
					break;
				}
				case SDL_EVENT_KEY_DOWN:
				{
					if (m_Event.key.windowID == windowID)
					{
						int repeatCount = m_Event.key.repeat ? 1 : 0;
						KeyPressedEvent event(m_Event.key.scancode, repeatCount);
						m_Data.EventCallback(event);
					}
					break;
				}
				case SDL_EVENT_KEY_UP:
				{
					if (m_Event.key.windowID == windowID)
					{
						KeyReleasedEvent event(m_Event.key.scancode);
						m_Data.EventCallback(event);
					}
					break;
				}
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
				{
					if (m_Event.button.windowID == windowID)
					{
						MouseButtonPressedEvent event(m_Event.button.button);
						m_Data.EventCallback(event);
					}
					break;
				}
				case SDL_EVENT_MOUSE_BUTTON_UP:
				{
					if (m_Event.button.windowID == windowID)
					{
						MouseButtonReleasedEvent event(m_Event.button.button);
						m_Data.EventCallback(event);
					}
					break;
				}
				case SDL_EVENT_MOUSE_WHEEL:
				{
					if (m_Event.wheel.windowID == windowID)
					{
						MouseScrolledEvent event(m_Event.wheel.x, m_Event.wheel.y);
						m_Data.EventCallback(event);
					}
					break;
				}
				case SDL_EVENT_MOUSE_MOTION:
				{
					if (m_Event.motion.windowID == windowID)
					{
						MouseMovedEvent event(m_Event.motion.x, m_Event.motion.y);
						m_Data.EventCallback(event);
					}
					break;
				}
			}
		}
    }

    void Window::Shutdown()
    {
    	if (m_Window)
    	{
    		SDL_DestroyWindow(m_Window);
    		m_Window = nullptr;
    	}
    	SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    }

	std::pair<float, float> Window::GetWindowPos() const
    {
    	int x, y;
    	if (SDL_GetWindowPosition(m_Window, &x, &y) == false)
    	{
    		SDLErrorCallback("SDL_GetWindowPosition");
    		return { 0.0f, 0.0f };
    	}
    	return { static_cast<float>(x), static_cast<float>(y) };
    }

    void Window::ProcessEvents()
    {
    	PollEvents();
    }

    void Window::SwapBuffers()
    {
        // swapchain
    }

	void Window::SetVSync(bool enabled)
    {
    	m_Specification.VSync = enabled;

    	// swapchain
    }

	bool Window::IsVSync() const
    {
    	return m_Specification.VSync;
    }

	void Window::SetResizable(bool resizable) const
    {
    	if (SDL_SetWindowResizable(m_Window, resizable) == false)
    	{
    		SDLErrorCallback("SDL_SetWindowResizable");
    	}
    }

	void Window::Maximize()
    {
    	if (m_Specification.Mode == WindowMode::Windowed)
    	{
    		if (SDL_MaximizeWindow(m_Window) == false)
    		{
    			SDLErrorCallback("SDL_MaximizeWindow");
    		}
    	}
    }

	void Window::CenterWindow()
    {
    	SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
    	if (displayID == 0)
    	{
    		SDLErrorCallback("SDL_GetPrimaryDisplay");
    		return;
    	}

    	const SDL_DisplayMode* videomode = SDL_GetCurrentDisplayMode(displayID);
    	if (!videomode)
    	{
    		SDLErrorCallback("SDL_GetCurrentDisplayMode");
    		return;
    	}

    	int x = (videomode->w / 2) - (m_Data.Width / 2);
    	int y = (videomode->h / 2) - (m_Data.Height / 2);

    	if (SDL_SetWindowPosition(m_Window, x, y) == false)
    	{
    		SDLErrorCallback("SDL_SetWindowPosition");
    	}
    }

    void Window::SetTitle(const std::string& title)
    {
        m_Data.Title = title;
    	if (SDL_SetWindowTitle(m_Window, title.c_str()) == false)
    	{
    		SDLErrorCallback("SDL_SetWindowTitle");
    	}
    }

}