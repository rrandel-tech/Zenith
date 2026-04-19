#include "znpch.hpp"
#include "Window.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Input.hpp"

#include <stb_image.h>

namespace Zenith {

#include "Embed/ZenithIcon.embed"

    static void SDLErrorCallback(const char* context)
    {
        ZN_CORE_ERROR_TAG("Window", "SDL Error ({}): {}", context, SDL_GetError());
    }

    Window* Window::Create(const WindowSpecification& specification)
    {
    	return new Window(specification);
    }

    Window::Window(const WindowSpecification& props)
        : m_Specification(props), m_Data()
    {
    }

    Window::~Window()
    {
        Shutdown();
    }

	void Window::Init()
    {
    	m_Data.Title  = m_Specification.Title;
    	m_Data.Width  = m_Specification.Width;
    	m_Data.Height = m_Specification.Height;

    	ZN_CORE_INFO_TAG("Window", "Creating window {0} ({1}, {2})", m_Specification.Title, m_Specification.Width, m_Specification.Height);

    	SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD);

    	Uint32 windowFlags = SDL_WINDOW_HIGH_PIXEL_DENSITY;

    	switch (m_Specification.Mode)
    	{
    		case WindowMode::Windowed:
    			m_Window = SDL_CreateWindow(m_Data.Title.c_str(), m_Data.Width, m_Data.Height, windowFlags);
    			break;
    		case WindowMode::BorderlessFullscreen:
    		{
    			SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
    			const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(displayID);
    			m_Window = SDL_CreateWindow(m_Data.Title.c_str(), mode->w, mode->h, windowFlags | SDL_WINDOW_BORDERLESS);
    			SDL_SetWindowPosition(m_Window, 0, 0);
    			break;
    		}
    		case WindowMode::ExclusiveFullscreen:
    		{
    			SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
    			const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(displayID);
    			m_Window = SDL_CreateWindow(m_Data.Title.c_str(), mode->w, mode->h, windowFlags | SDL_WINDOW_FULLSCREEN);
    			break;
    		}
    		case WindowMode::Maximized:
    			m_Window = SDL_CreateWindow(m_Data.Title.c_str(), m_Data.Width, m_Data.Height, windowFlags | SDL_WINDOW_MAXIMIZED);
    			break;
    	}

    	ZN_CORE_ASSERT(m_Window, "Could not create window: {}", SDL_GetError());

    	// Set icon
	    {
    		int width, height, channels;
    		unsigned char* pixels = nullptr;

    		bool useEmbedded = m_Specification.IconPath.empty();

    		if (!useEmbedded)
    		{
    			std::string iconPathStr = m_Specification.IconPath.string();
    			pixels = stbi_load(iconPathStr.c_str(), &width, &height, &channels, 4);
    			if (!pixels)
    				useEmbedded = true;
    		}

    		if (useEmbedded)
    			pixels = stbi_load_from_memory(g_ZenithIconPNG, sizeof(g_ZenithIconPNG), &width, &height, &channels, 4);

    		if (pixels)
    		{
    			SDL_Surface* icon = SDL_CreateSurfaceFrom(width, height, SDL_PIXELFORMAT_RGBA32, pixels, width * 4);
    			if (icon)
    			{
    				SDL_SetWindowIcon(m_Window, icon);
    				SDL_DestroySurface(icon);
    			}
    			stbi_image_free(pixels);
    		}
	    }

    	int width, height;
    	SDL_GetWindowSize(m_Window, &width, &height);
    	m_Data.Width  = width;
    	m_Data.Height = height;
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
	                    m_Data.Width = width;
	                    m_Data.Height = height;

	                    WindowResizeEvent event(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
	                    m_Data.EventCallback(event);
	                }
	                break;
	            }
	            case SDL_EVENT_WINDOW_MINIMIZED:
	            {
	                if (m_Event.window.windowID == windowID)
	                {
	                    WindowMinimizeEvent event(true);
	                    m_Data.EventCallback(event);
	                }
	                break;
	            }
	        	case SDL_EVENT_WINDOW_MAXIMIZED:
	            {
	            	if (m_Event.window.windowID == windowID)
	            	{
	            		WindowMinimizeEvent event(false);
	            		m_Data.EventCallback(event);
	            	}
	            	break;
	            }
	            case SDL_EVENT_WINDOW_RESTORED:
	            {
	                if (m_Event.window.windowID == windowID)
	                {
	                    WindowMinimizeEvent event(false);
	                    m_Data.EventCallback(event);
	                }
	                break;
	            }
	            case SDL_EVENT_KEY_DOWN:
	            {
	                if (m_Event.key.windowID == windowID)
	                {
	                	Input::UpdateKeyMods(static_cast<KeyMods>(m_Event.key.mod));

	                    KeyCode key = static_cast<KeyCode>(m_Event.key.scancode);
	                    if (m_Event.key.repeat)
	                    {
	                        Input::UpdateKeyState(key, KeyState::Held);
	                        KeyPressedEvent event(key, 1);
	                        m_Data.EventCallback(event);
	                    }
	                    else
	                    {
	                        Input::UpdateKeyState(key, KeyState::Pressed);
	                        KeyPressedEvent event(key, 0);
	                        m_Data.EventCallback(event);
	                    }
	                }
	                break;
	            }
	            case SDL_EVENT_KEY_UP:
	            {
	                if (m_Event.key.windowID == windowID)
	                {
	                	Input::UpdateKeyMods(static_cast<KeyMods>(m_Event.key.mod));

	                    KeyCode key = static_cast<KeyCode>(m_Event.key.scancode);
	                    Input::UpdateKeyState(key, KeyState::Released);
	                    KeyReleasedEvent event(key);
	                    m_Data.EventCallback(event);
	                }
	                break;
	            }
	            case SDL_EVENT_TEXT_INPUT:
	            {
	                if (m_Event.text.windowID == windowID)
	                {
	                    const char* text = m_Event.text.text;
	                    uint32_t codepoint = 0;

	                    unsigned char c = static_cast<unsigned char>(text[0]);
	                    if (c < 0x80)
	                        codepoint = c;
	                    else if (c < 0xE0)
	                        codepoint = ((c & 0x1F) << 6) | (text[1] & 0x3F);
	                    else if (c < 0xF0)
	                        codepoint = ((c & 0x0F) << 12) | ((text[1] & 0x3F) << 6) | (text[2] & 0x3F);

	                    KeyTypedEvent event(static_cast<KeyCode>(codepoint));
	                    m_Data.EventCallback(event);
	                }
	                break;
	            }
	            case SDL_EVENT_MOUSE_BUTTON_DOWN:
	            {
	                if (m_Event.button.windowID == windowID)
	                {
	                    MouseButton button = static_cast<MouseButton>(m_Event.button.button);
	                    Input::UpdateButtonState(button, KeyState::Pressed);
	                    MouseButtonPressedEvent event(button);
	                    m_Data.EventCallback(event);
	                }
	                break;
	            }
	            case SDL_EVENT_MOUSE_BUTTON_UP:
	            {
	                if (m_Event.button.windowID == windowID)
	                {
	                    MouseButton button = static_cast<MouseButton>(m_Event.button.button);
	                    Input::UpdateButtonState(button, KeyState::Released);
	                    MouseButtonReleasedEvent event(button);
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
	            		if (SDL_GetWindowRelativeMouseMode(m_Window))
	            		{
	            			MouseMovedEvent event(m_Event.motion.xrel, m_Event.motion.yrel);
	            			m_Data.EventCallback(event);
	            		}
	            		else
	            		{
	            			MouseMovedEvent event(m_Event.motion.x, m_Event.motion.y);
	            			m_Data.EventCallback(event);
	            		}
	            	}
	            	break;
	            }
	        	case SDL_EVENT_GAMEPAD_ADDED:
	            {
	            	SDL_Gamepad* gamepad = SDL_OpenGamepad(m_Event.gdevice.which);
	            	if (!gamepad)
	            		SDLErrorCallback("SDL_OpenGamepad");
	            	break;
	            }
	        	case SDL_EVENT_GAMEPAD_REMOVED:
	            {
	            	SDL_Gamepad* gamepad = SDL_GetGamepadFromID(m_Event.gdevice.which);
	            	if (gamepad)
	            		SDL_CloseGamepad(gamepad);
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
    	SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD);
    }

	std::pair<float, float> Window::GetWindowPos() const
    {
    	int x, y;
    	if (!SDL_GetWindowPosition(m_Window, &x, &y))
    	{
    		ZN_CORE_WARN_TAG("Window", "Failed to get window position: {}", SDL_GetError());
    		return { 0.0f, 0.0f };
    	}
    	return { static_cast<float>(x), static_cast<float>(y) };
    }

    void Window::ProcessEvents()
    {
    	PollEvents();
    	Input::Update();
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
    	if (!SDL_SetWindowResizable(m_Window, resizable))
    		SDLErrorCallback("SDL_SetWindowResizable");
    }

	void Window::Maximize()
    {
    	if (m_Specification.Mode == WindowMode::Windowed || m_Specification.Mode == WindowMode::Maximized)
    		SDL_MaximizeWindow(m_Window);
    }

	void Window::CenterWindow()
    {
    	SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }

    void Window::SetTitle(const std::string& title)
    {
    	m_Data.Title = title;
    	if (!SDL_SetWindowTitle(m_Window, title.c_str()))
    		SDLErrorCallback("SDL_SetWindowTitle");
    }

}