#pragma once

#include "gfxengine/math.hpp"

#include <vector>
#include <string_view>
#include <ranges>

struct Image
{
	enum class Format
	{
		RGBA,
	};

	std::vector<uint8_t> data;
	size_t width = 0;
	size_t height = 0;
	Format format = Format::RGBA;

	static Image load_sync(std::string_view file_name);
	static Image load(std::span<const uint8_t> file_data);
};
