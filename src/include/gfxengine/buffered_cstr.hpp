#pragma once

#include <format>

template <size_t N = 1024>
struct BufferedCStr
{
	char buf[N];
	char *str;
	size_t size;

	constexpr bool is_allocated() const { return str != buf; }

	constexpr BufferedCStr(size_t _size)
	{
		size = _size;

		str = buf;

		if (size + 1 > N)
			str = (char *)malloc(size + 1);

		str[size] = '\0';
	}

	BufferedCStr(BufferedCStr &other) = delete;
	BufferedCStr &operator = (BufferedCStr &other) = delete;

	constexpr BufferedCStr(BufferedCStr &&other)
	{
		size = other.size;

		if (other.is_allocated())
		{
			str = other.str;
			other.str = nullptr;
		}
		else
		{
			str = buf;

			for (size_t i = 0; i < size; ++i)
				str[i] = other.buf[i];

			str[size] = '\0';
		}
	}
	constexpr BufferedCStr &operator = (BufferedCStr &&other)
	{
		size = other.size;

		if (other.is_allocated())
		{
			if (is_allocated())
			{
				std::swap(str, str);
			}
			else
			{
				str = other.str;
				other.str = nullptr;
			}
		}
		else
		{
			if (is_allocated())
				free(str);

			str = buf;

			for (size_t i = 0; i < size; ++i)
				str[i] = other.buf[i];

			str[size] = '\0';
		}

		return *this;
	}

	constexpr ~BufferedCStr()
	{
		if (is_allocated())
			free(str);
	}

	[[nodiscard]] constexpr char const *c_str() const { return str; }
	[[nodiscard]] constexpr size_t len() const { return size; }

	template <class... _Types>
	[[nodiscard]]
	static constexpr BufferedCStr format(std::format_string<const _Types &...> _Fmt, const _Types &... _Args)
	{
		BufferedCStr result(0);

		result.str = result.buf;
		auto r = std::format_to_n(result.str, (size_t)N - 1, _Fmt, _Args...);

		result.size = r.size;

		if (result.size + 1 > N)
		{
			result.str = (char *)malloc(result.size + 1);
			auto r = std::format_to_n(result.str, (size_t)N - 1, _Fmt, _Args...);
		}

		result.str[result.size] = '\0';

		return result;
	}
};
