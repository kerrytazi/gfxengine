#pragma once

#include "gfxengine/math.hpp"

struct KeyboardEvent
{
	enum class Type : uint8_t
	{
		Press,
		Release,
		Repeat,
	};

	enum class Key : uint8_t
	{
		None,
		Space, Apostrophe, Comma, Minus, Period, Slash, Semicolon, Equal,
		LeftBracket, Backslash, RightBracket, GraveAccent,
		K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		Escape, Enter, Tab, Backspace, Insert, Delete,
		Left, Up, Right, Down,
		PageUp, PageDown, Home, End,
		CapsLock, ScrollLock, NumLock, PrintScreen, Pause,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,
		Kp0, Kp1, Kp2, Kp3, Kp4, Kp5, Kp6, Kp7, Kp8, Kp9,
		KpDecimal, KpDivide, KpMultiply, KpSubtract, KpAdd, KpEnter, KpEqual,
		LeftShift, LeftControl, LeftAlt, LeftSuper,
		RightShift, RightControl, RightAlt, RightSuper,
		Menu,
	};

	Type type;
	Key key;
	bool locked;
};

struct MouseEvent
{
	enum class Type : uint8_t
	{
		Press,
		Release,
		Move,
		Scroll,
	};

	enum class Key : uint8_t
	{
		None,
		LMB,
		RMB,
		MMB,
		K_1 = LMB,
		K_2 = RMB,
		K_3 = MMB,
		K_4,
		K_5,
		K_6,
		K_7,
		K_8,
	};

	Type type;
	Key key;
	bool locked;
	glm::vec2 pos; // delta if locked
	glm::vec2 scroll;
};

class WindowEventHandler
{
public:

	virtual ~WindowEventHandler() = default;

	virtual void on_keyboard_event(double time, KeyboardEvent event)
	{
	}

	virtual void on_mouse_event(double time, MouseEvent event)
	{
	}

	virtual void on_close_event(double time)
	{
	}
};
