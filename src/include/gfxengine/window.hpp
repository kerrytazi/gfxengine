#pragma once

class Frame;
class Platform;
class Graphics;
class WindowEventHandler;

struct CreateWindowParams
{
	Platform &platform;
	WindowEventHandler *window_event_handler = nullptr;
};

class Window
{
public:

	virtual ~Window() = default;

	virtual void poll_events() = 0;
	virtual void draw(Frame const &frame) = 0;
	virtual Graphics &get_graphics() const = 0;

	virtual void lock_mouse(bool lock) = 0;
	virtual void close() = 0;
	virtual void set_vsync(bool enable) = 0;
};
