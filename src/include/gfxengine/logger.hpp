#pragma once

#include "gfxengine/platform.hpp"
#include "gfxengine/buffered_cstr.hpp"

#include <vector>
#include <functional>

class Logger
{
public:

	enum class Level
	{
		Info,
		Warning,
		Error,
	};

	using HandlerFunc = std::function<void(char const *c_str, size_t len)>;

	Logger(Platform &_platform)
		: platform{ _platform }
	{
	}

	template <class... _Types>
	void log(std::format_string<const _Types &...> _Fmt, const _Types &... _Args)
	{
		handlet(Level::Info, _Fmt, _Args...);
	}

	template <class... _Types>
	void logw(std::format_string<const _Types &...> _Fmt, const _Types &... _Args)
	{
		handlet(Level::Warning, _Fmt, _Args...);
	}

	template <class... _Types>
	void loge(std::format_string<const _Types &...> _Fmt, const _Types &... _Args)
	{
		handlet(Level::Error, _Fmt, _Args...);
	}

	template <class... _Types>
	void logd(std::format_string<const _Types &...> _Fmt, const _Types &... _Args)
	{
#ifndef NDEBUG
		handlet(Level::Info, _Fmt, _Args...);
#endif // !NDEBUG
	}

	template <class... _Types>
	void logdw(std::format_string<const _Types &...> _Fmt, const _Types &... _Args)
	{
#ifndef NDEBUG
		handlet(Level::Warning, _Fmt, _Args...);
#endif // !NDEBUG
	}

	template <class... _Types>
	void logde(std::format_string<const _Types &...> _Fmt, const _Types &... _Args)
	{
#ifndef NDEBUG
		handlet(Level::Error, _Fmt, _Args...);
#endif // !NDEBUG
	}

	int add_handler(HandlerFunc handler)
	{
		++last_handler_id;
		handlers.emplace_back(last_handler_id, std::move(handler));
		return last_handler_id;
	}

	void remove_handler(int id)
	{
		auto it = std::find_if(handlers.begin(), handlers.end(), [&](auto &v) { return v.id == id; });

		if (it != handlers.end())
			handlers.erase(it);
	}

private:

	struct HandlerWithID
	{
		int id;
		HandlerFunc handler;
	};

	Platform &platform;
	std::vector<HandlerWithID> handlers;
	int last_handler_id = 0;

	template <class... _Types>
	void handlet(Level level, std::format_string<const _Types &...> _Fmt, const _Types &... _Args)
	{
		if (handlers.empty()) return;
		auto buf = BufferedCStr<>::format(_Fmt, _Args...);
		handle(level, buf.c_str(), buf.len());
	}

	void handle(Level level, char const *c_str, size_t len);
};
