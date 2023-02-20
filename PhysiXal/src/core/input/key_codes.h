#pragma once

namespace PhysiXal {

	typedef enum class KeyCode : uint16_t
	{
		// Key codes
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

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define PX_KEY_SPACE           ::PhysiXal::Key::Space
#define PX_KEY_APOSTROPHE      ::PhysiXal::Key::Apostrophe    /* ' */
#define PX_KEY_COMMA           ::PhysiXal::Key::Comma         /* , */
#define PX_KEY_MINUS           ::PhysiXal::Key::Minus         /* - */
#define PX_KEY_PERIOD          ::PhysiXal::Key::Period        /* . */
#define PX_KEY_SLASH           ::PhysiXal::Key::Slash         /* / */
#define PX_KEY_0               ::PhysiXal::Key::D0
#define PX_KEY_1               ::PhysiXal::Key::D1
#define PX_KEY_2               ::PhysiXal::Key::D2
#define PX_KEY_3               ::PhysiXal::Key::D3
#define PX_KEY_4               ::PhysiXal::Key::D4
#define PX_KEY_5               ::PhysiXal::Key::D5
#define PX_KEY_6               ::PhysiXal::Key::D6
#define PX_KEY_7               ::PhysiXal::Key::D7
#define PX_KEY_8               ::PhysiXal::Key::D8
#define PX_KEY_9               ::PhysiXal::Key::D9
#define PX_KEY_SEMICOLON       ::PhysiXal::Key::Semicolon     /* ; */
#define PX_KEY_EQUAL           ::PhysiXal::Key::Equal         /* = */
#define PX_KEY_A               ::PhysiXal::Key::A
#define PX_KEY_B               ::PhysiXal::Key::B
#define PX_KEY_C               ::PhysiXal::Key::C
#define PX_KEY_D               ::PhysiXal::Key::D
#define PX_KEY_E               ::PhysiXal::Key::E
#define PX_KEY_F               ::PhysiXal::Key::F
#define PX_KEY_G               ::PhysiXal::Key::G
#define PX_KEY_H               ::PhysiXal::Key::H
#define PX_KEY_I               ::PhysiXal::Key::I
#define PX_KEY_J               ::PhysiXal::Key::J
#define PX_KEY_K               ::PhysiXal::Key::K
#define PX_KEY_L               ::PhysiXal::Key::L
#define PX_KEY_M               ::PhysiXal::Key::M
#define PX_KEY_N               ::PhysiXal::Key::N
#define PX_KEY_O               ::PhysiXal::Key::O
#define PX_KEY_P               ::PhysiXal::Key::P
#define PX_KEY_Q               ::PhysiXal::Key::Q
#define PX_KEY_R               ::PhysiXal::Key::R
#define PX_KEY_S               ::PhysiXal::Key::S
#define PX_KEY_T               ::PhysiXal::Key::T
#define PX_KEY_U               ::PhysiXal::Key::U
#define PX_KEY_V               ::PhysiXal::Key::V
#define PX_KEY_W               ::PhysiXal::Key::W
#define PX_KEY_X               ::PhysiXal::Key::X
#define PX_KEY_Y               ::PhysiXal::Key::Y
#define PX_KEY_Z               ::PhysiXal::Key::Z
#define PX_KEY_LEFT_BRACKET    ::PhysiXal::Key::LeftBracket   /* [ */
#define PX_KEY_BACKSLASH       ::PhysiXal::Key::Backslash     /* \ */
#define PX_KEY_RIGHT_BRACKET   ::PhysiXal::Key::RightBracket  /* ] */
#define PX_KEY_GRAVE_ACCENT    ::PhysiXal::Key::GraveAccent   /* ` */
#define PX_KEY_WORLD_1         ::PhysiXal::Key::World1        /* non-US #1 */
#define PX_KEY_WORLD_2         ::PhysiXal::Key::World2        /* non-US #2 */

/* Function keys */
#define PX_KEY_ESCAPE          ::PhysiXal::Key::Escape
#define PX_KEY_ENTER           ::PhysiXal::Key::Enter
#define PX_KEY_TAB             ::PhysiXal::Key::Tab
#define PX_KEY_BACKSPACE       ::PhysiXal::Key::Backspace
#define PX_KEY_INSERT          ::PhysiXal::Key::Insert
#define PX_KEY_DELETE          ::PhysiXal::Key::Delete
#define PX_KEY_RIGHT           ::PhysiXal::Key::Right
#define PX_KEY_LEFT            ::PhysiXal::Key::Left
#define PX_KEY_DOWN            ::PhysiXal::Key::Down
#define PX_KEY_UP              ::PhysiXal::Key::Up
#define PX_KEY_PAGE_UP         ::PhysiXal::Key::PageUp
#define PX_KEY_PAGE_DOWN       ::PhysiXal::Key::PageDown
#define PX_KEY_HOME            ::PhysiXal::Key::Home
#define PX_KEY_END             ::PhysiXal::Key::End
#define PX_KEY_CAPS_LOCK       ::PhysiXal::Key::CapsLock
#define PX_KEY_SCROLL_LOCK     ::PhysiXal::Key::ScrollLock
#define PX_KEY_NUM_LOCK        ::PhysiXal::Key::NumLock
#define PX_KEY_PRINT_SCREEN    ::PhysiXal::Key::PrintScreen
#define PX_KEY_PAUSE           ::PhysiXal::Key::Pause
#define PX_KEY_F1              ::PhysiXal::Key::F1
#define PX_KEY_F2              ::PhysiXal::Key::F2
#define PX_KEY_F3              ::PhysiXal::Key::F3
#define PX_KEY_F4              ::PhysiXal::Key::F4
#define PX_KEY_F5              ::PhysiXal::Key::F5
#define PX_KEY_F6              ::PhysiXal::Key::F6
#define PX_KEY_F7              ::PhysiXal::Key::F7
#define PX_KEY_F8              ::PhysiXal::Key::F8
#define PX_KEY_F9              ::PhysiXal::Key::F9
#define PX_KEY_F10             ::PhysiXal::Key::F10
#define PX_KEY_F11             ::PhysiXal::Key::F11
#define PX_KEY_F12             ::PhysiXal::Key::F12
#define PX_KEY_F13             ::PhysiXal::Key::F13
#define PX_KEY_F14             ::PhysiXal::Key::F14
#define PX_KEY_F15             ::PhysiXal::Key::F15
#define PX_KEY_F16             ::PhysiXal::Key::F16
#define PX_KEY_F17             ::PhysiXal::Key::F17
#define PX_KEY_F18             ::PhysiXal::Key::F18
#define PX_KEY_F19             ::PhysiXal::Key::F19
#define PX_KEY_F20             ::PhysiXal::Key::F20
#define PX_KEY_F21             ::PhysiXal::Key::F21
#define PX_KEY_F22             ::PhysiXal::Key::F22
#define PX_KEY_F23             ::PhysiXal::Key::F23
#define PX_KEY_F24             ::PhysiXal::Key::F24
#define PX_KEY_F25             ::PhysiXal::Key::F25

/* Keypad */
#define PX_KEY_KP_0            ::PhysiXal::Key::KP0
#define PX_KEY_KP_1            ::PhysiXal::Key::KP1
#define PX_KEY_KP_2            ::PhysiXal::Key::KP2
#define PX_KEY_KP_3            ::PhysiXal::Key::KP3
#define PX_KEY_KP_4            ::PhysiXal::Key::KP4
#define PX_KEY_KP_5            ::PhysiXal::Key::KP5
#define PX_KEY_KP_6            ::PhysiXal::Key::KP6
#define PX_KEY_KP_7            ::PhysiXal::Key::KP7
#define PX_KEY_KP_8            ::PhysiXal::Key::KP8
#define PX_KEY_KP_9            ::PhysiXal::Key::KP9
#define PX_KEY_KP_DECIMAL      ::PhysiXal::Key::KPDecimal
#define PX_KEY_KP_DIVIDE       ::PhysiXal::Key::KPDivide
#define PX_KEY_KP_MULTIPLY     ::PhysiXal::Key::KPMultiply
#define PX_KEY_KP_SUBTRACT     ::PhysiXal::Key::KPSubtract
#define PX_KEY_KP_ADD          ::PhysiXal::Key::KPAdd
#define PX_KEY_KP_ENTER        ::PhysiXal::Key::KPEnter
#define PX_KEY_KP_EQUAL        ::PhysiXal::Key::KPEqual

#define PX_KEY_LEFT_SHIFT      ::PhysiXal::Key::LeftShift
#define PX_KEY_LEFT_CONTROL    ::PhysiXal::Key::LeftControl
#define PX_KEY_LEFT_ALT        ::PhysiXal::Key::LeftAlt
#define PX_KEY_LEFT_SUPER      ::PhysiXal::Key::LeftSuper
#define PX_KEY_RIGHT_SHIFT     ::PhysiXal::Key::RightShift
#define PX_KEY_RIGHT_CONTROL   ::PhysiXal::Key::RightControl
#define PX_KEY_RIGHT_ALT       ::PhysiXal::Key::RightAlt
#define PX_KEY_RIGHT_SUPER     ::PhysiXal::Key::RightSuper
#define PX_KEY_MENU            ::PhysiXal::Key::Menu