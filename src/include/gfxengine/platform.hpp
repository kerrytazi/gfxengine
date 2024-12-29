#pragma once

#include <memory>

class Window;
class CreateWindowParams;

class Platform
{
public:

	virtual ~Platform() = default;

	[[nodiscard]]
	virtual std::unique_ptr<Window> create_window(CreateWindowParams const &params) const = 0;

	[[nodiscard]]
	virtual double get_time() const = 0;
	virtual void sleep(double seconds) = 0;
};
