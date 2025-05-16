#pragma once

#include <ostream>
#include <cstdint>

namespace Zenith {
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	enum class KeyState
	{
		None = -1,
		Pressed,
		Held,
		Released
	};

	enum class CursorMode
	{
		Normal = 0,
		Hidden = 1,
		Locked = 2
	};

	typedef enum class MouseButton : uint16_t
	{
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Left = Button0,
		Right = Button1,
		Middle = Button2
	} Button;


	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, MouseButton button)
	{
		os << static_cast<int32_t>(button);
		return os;
	}
}

// From glfw3.h
#define ZN_KEY_SPACE           ::Zenith::Key::Space
#define ZN_KEY_APOSTROPHE      ::Zenith::Key::Apostrophe    /* ' */
#define ZN_KEY_COMMA           ::Zenith::Key::Comma         /* , */
#define ZN_KEY_MINUS           ::Zenith::Key::Minus         /* - */
#define ZN_KEY_PERIOD          ::Zenith::Key::Period        /* . */
#define ZN_KEY_SLASH           ::Zenith::Key::Slash         /* / */
#define ZN_KEY_0               ::Zenith::Key::D0
#define ZN_KEY_1               ::Zenith::Key::D1
#define ZN_KEY_2               ::Zenith::Key::D2
#define ZN_KEY_3               ::Zenith::Key::D3
#define ZN_KEY_4               ::Zenith::Key::D4
#define ZN_KEY_5               ::Zenith::Key::D5
#define ZN_KEY_6               ::Zenith::Key::D6
#define ZN_KEY_7               ::Zenith::Key::D7
#define ZN_KEY_8               ::Zenith::Key::D8
#define ZN_KEY_9               ::Zenith::Key::D9
#define ZN_KEY_SEMICOLON       ::Zenith::Key::Semicolon     /* ; */
#define ZN_KEY_EQUAL           ::Zenith::Key::Equal         /* = */
#define ZN_KEY_A               ::Zenith::Key::A
#define ZN_KEY_B               ::Zenith::Key::B
#define ZN_KEY_C               ::Zenith::Key::C
#define ZN_KEY_D               ::Zenith::Key::D
#define ZN_KEY_E               ::Zenith::Key::E
#define ZN_KEY_F               ::Zenith::Key::F
#define ZN_KEY_G               ::Zenith::Key::G
#define ZN_KEY_H               ::Zenith::Key::H
#define ZN_KEY_I               ::Zenith::Key::I
#define ZN_KEY_J               ::Zenith::Key::J
#define ZN_KEY_K               ::Zenith::Key::K
#define ZN_KEY_L               ::Zenith::Key::L
#define ZN_KEY_M               ::Zenith::Key::M
#define ZN_KEY_N               ::Zenith::Key::N
#define ZN_KEY_O               ::Zenith::Key::O
#define ZN_KEY_P               ::Zenith::Key::P
#define ZN_KEY_Q               ::Zenith::Key::Q
#define ZN_KEY_R               ::Zenith::Key::R
#define ZN_KEY_S               ::Zenith::Key::S
#define ZN_KEY_T               ::Zenith::Key::T
#define ZN_KEY_U               ::Zenith::Key::U
#define ZN_KEY_V               ::Zenith::Key::V
#define ZN_KEY_W               ::Zenith::Key::W
#define ZN_KEY_X               ::Zenith::Key::X
#define ZN_KEY_Y               ::Zenith::Key::Y
#define ZN_KEY_Z               ::Zenith::Key::Z
#define ZN_KEY_LEFT_BRACKET    ::Zenith::Key::LeftBracket   /* [ */
#define ZN_KEY_BACKSLASH       ::Zenith::Key::Backslash     /* \ */
#define ZN_KEY_RIGHT_BRACKET   ::Zenith::Key::RightBracket  /* ] */
#define ZN_KEY_GRAVE_ACCENT    ::Zenith::Key::GraveAccent   /* ` */
#define ZN_KEY_WORLD_1         ::Zenith::Key::World1        /* non-US #1 */
#define ZN_KEY_WORLD_2         ::Zenith::Key::World2        /* non-US #2 */

/* Function keys */
#define ZN_KEY_ESCAPE          ::Zenith::Key::Escape
#define ZN_KEY_ENTER           ::Zenith::Key::Enter
#define ZN_KEY_TAB             ::Zenith::Key::Tab
#define ZN_KEY_BACKSPACE       ::Zenith::Key::Backspace
#define ZN_KEY_INSERT          ::Zenith::Key::Insert
#define ZN_KEY_DELETE          ::Zenith::Key::Delete
#define ZN_KEY_RIGHT           ::Zenith::Key::Right
#define ZN_KEY_LEFT            ::Zenith::Key::Left
#define ZN_KEY_DOWN            ::Zenith::Key::Down
#define ZN_KEY_UP              ::Zenith::Key::Up
#define ZN_KEY_PAGE_UP         ::Zenith::Key::PageUp
#define ZN_KEY_PAGE_DOWN       ::Zenith::Key::PageDown
#define ZN_KEY_HOME            ::Zenith::Key::Home
#define ZN_KEY_END             ::Zenith::Key::End
#define ZN_KEY_CAPS_LOCK       ::Zenith::Key::CapsLock
#define ZN_KEY_SCROLL_LOCK     ::Zenith::Key::ScrollLock
#define ZN_KEY_NUM_LOCK        ::Zenith::Key::NumLock
#define ZN_KEY_PRINT_SCREEN    ::Zenith::Key::PrintScreen
#define ZN_KEY_PAUSE           ::Zenith::Key::Pause
#define ZN_KEY_F1              ::Zenith::Key::F1
#define ZN_KEY_F2              ::Zenith::Key::F2
#define ZN_KEY_F3              ::Zenith::Key::F3
#define ZN_KEY_F4              ::Zenith::Key::F4
#define ZN_KEY_F5              ::Zenith::Key::F5
#define ZN_KEY_F6              ::Zenith::Key::F6
#define ZN_KEY_F7              ::Zenith::Key::F7
#define ZN_KEY_F8              ::Zenith::Key::F8
#define ZN_KEY_F9              ::Zenith::Key::F9
#define ZN_KEY_F10             ::Zenith::Key::F10
#define ZN_KEY_F11             ::Zenith::Key::F11
#define ZN_KEY_F12             ::Zenith::Key::F12
#define ZN_KEY_F13             ::Zenith::Key::F13
#define ZN_KEY_F14             ::Zenith::Key::F14
#define ZN_KEY_F15             ::Zenith::Key::F15
#define ZN_KEY_F16             ::Zenith::Key::F16
#define ZN_KEY_F17             ::Zenith::Key::F17
#define ZN_KEY_F18             ::Zenith::Key::F18
#define ZN_KEY_F19             ::Zenith::Key::F19
#define ZN_KEY_F20             ::Zenith::Key::F20
#define ZN_KEY_F21             ::Zenith::Key::F21
#define ZN_KEY_F22             ::Zenith::Key::F22
#define ZN_KEY_F23             ::Zenith::Key::F23
#define ZN_KEY_F24             ::Zenith::Key::F24
#define ZN_KEY_F25             ::Zenith::Key::F25

/* Keypad */
#define ZN_KEY_KP_0            ::Zenith::Key::KP0
#define ZN_KEY_KP_1            ::Zenith::Key::KP1
#define ZN_KEY_KP_2            ::Zenith::Key::KP2
#define ZN_KEY_KP_3            ::Zenith::Key::KP3
#define ZN_KEY_KP_4            ::Zenith::Key::KP4
#define ZN_KEY_KP_5            ::Zenith::Key::KP5
#define ZN_KEY_KP_6            ::Zenith::Key::KP6
#define ZN_KEY_KP_7            ::Zenith::Key::KP7
#define ZN_KEY_KP_8            ::Zenith::Key::KP8
#define ZN_KEY_KP_9            ::Zenith::Key::KP9
#define ZN_KEY_KP_DECIMAL      ::Zenith::Key::KPDecimal
#define ZN_KEY_KP_DIVIDE       ::Zenith::Key::KPDivide
#define ZN_KEY_KP_MULTIPLY     ::Zenith::Key::KPMultiply
#define ZN_KEY_KP_SUBTRACT     ::Zenith::Key::KPSubtract
#define ZN_KEY_KP_ADD          ::Zenith::Key::KPAdd
#define ZN_KEY_KP_ENTER        ::Zenith::Key::KPEnter
#define ZN_KEY_KP_EQUAL        ::Zenith::Key::KPEqual

#define ZN_KEY_LEFT_SHIFT      ::Zenith::Key::LeftShift
#define ZN_KEY_LEFT_CONTROL    ::Zenith::Key::LeftControl
#define ZN_KEY_LEFT_ALT        ::Zenith::Key::LeftAlt
#define ZN_KEY_LEFT_SUPER      ::Zenith::Key::LeftSuper
#define ZN_KEY_RIGHT_SHIFT     ::Zenith::Key::RightShift
#define ZN_KEY_RIGHT_CONTROL   ::Zenith::Key::RightControl
#define ZN_KEY_RIGHT_ALT       ::Zenith::Key::RightAlt
#define ZN_KEY_RIGHT_SUPER     ::Zenith::Key::RightSuper
#define ZN_KEY_MENU            ::Zenith::Key::Menu

// Mouse
#define ZN_MOUSE_BUTTON_LEFT    ::Zenith::Button::Left
#define ZN_MOUSE_BUTTON_RIGHT   ::Zenith::Button::Right
#define ZN_MOUSE_BUTTON_MIDDLE  ::Zenith::Button::Middle
