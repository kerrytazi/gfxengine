#include "gfxengine/logger.hpp"

void Logger::handle(Level level, char const *c_str, size_t len)
{
	char const *level_str = "[I] ";
	if (level == Level::Warning) level_str = "[W] ";
	if (level == Level::Error) level_str = "[E] ";

	uint64_t t = platform.get_system_time_ms();

	uint64_t ms = t % 1000;
	uint64_t s = (t / 1000) % 60;
	uint64_t m = (t / 1000 / 60) % 60;
	uint64_t h = (t / 1000 / 60 / 60) % 24;

	auto buf = BufferedCStr<>::format("[{:02}:{:02}:{:02}.{:03}] {}{}\n", h, m, s, ms, level_str, std::string_view(c_str, len));

	for (auto &h : handlers)
		h.handler(buf.c_str(), buf.len());
}
