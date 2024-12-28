#include "gfxengine/platform.hpp"

#include "gfxengine/window.hpp"

#include "private/my_windows.hpp"

extern std::unique_ptr<Window> _create_window(CreateWindowParams const &params);

class WindowsPlatform : public Platform
{
private:

	double frequency = 0;

public:

	WindowsPlatform()
	{
		uint64_t value;
		(void)QueryPerformanceFrequency((LARGE_INTEGER *)&value);
		frequency = (double)value;
	}

	virtual std::unique_ptr<Window> create_window(CreateWindowParams const &params) const override
	{
		return _create_window(params);
	}

	virtual double get_time() const override
	{
		uint64_t value;
		QueryPerformanceCounter((LARGE_INTEGER *)&value);
		return (double)value / frequency;
	}
};

std::unique_ptr<Platform> _create_platform()
{
	return std::make_unique<WindowsPlatform>();
}
