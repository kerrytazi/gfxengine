#pragma once

class Frame;
class BasePlatform;
class BaseGraphics;
class WindowEventHandler;

struct CreateWindowParams
{
	BasePlatform &platform;
	WindowEventHandler *window_event_handler = nullptr;
};

class BaseWindow
{
public:

	virtual ~BaseWindow() = default;

	virtual void poll_events() = 0;
	virtual void draw(Frame const &frame) = 0;

	virtual void lock_mouse(bool lock) = 0;
	virtual void close() = 0;
	virtual void set_vsync(bool enable) = 0;
};
