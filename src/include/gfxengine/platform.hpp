#pragma once

#include <memory>

class Window;
struct CreateWindowParams;

class Platform
{
public:

	virtual ~Platform() = default;

	[[nodiscard]]
	virtual std::unique_ptr<Window> create_window(CreateWindowParams const &params) const = 0;
	
	[[nodiscard]]
	virtual uint64_t get_system_time_ms() const = 0;

	[[nodiscard]]
	virtual double get_time() const = 0;
	virtual void sleep(double seconds) = 0;

	virtual void debug_log(char const *c_str, size_t len) = 0;
};
