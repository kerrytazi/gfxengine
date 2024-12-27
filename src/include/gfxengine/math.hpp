#pragma once

#define _USE_MATH_DEFINES
#include <array>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>

enum class Line2IntersectResultType
{
	SegmentIntersect,
	LineIntersect,
	Collinear,
	Parallel,
};

struct Line2IntersectResult
{
	Line2IntersectResultType type;
	glm::vec2 pos;
	float phase0, phase1;

	constexpr bool Intersect() const
	{
		return type == Line2IntersectResultType::SegmentIntersect || type == Line2IntersectResultType::LineIntersect;
	}

	constexpr bool IntersectSegment1Forward() const
	{
		return type == Line2IntersectResultType::SegmentIntersect || (type == Line2IntersectResultType::LineIntersect && phase0 >= 0.0f && phase1 >= 0.0f && phase1 <= 1.0f);
	}
};

struct Line2
{
	glm::vec2 p0, p1;

	constexpr Line2IntersectResult Intersect(Line2 const &other) const
	{
		// https://youtu.be/bvlIYX9cgls

		const auto x0 = p0.x, x1 = p1.x, x2 = other.p0.x, x3 = other.p1.x;
		const auto y0 = p0.y, y1 = p1.y, y2 = other.p0.y, y3 = other.p1.y;

		const auto a = (x3 - x2)*(y2 - y0) - (y3 - y2)*(x2 - x0);
		const auto b = (x3 - x2)*(y1 - y0) - (y3 - y2)*(x1 - x0);
		const auto c = (x1 - x0)*(y2 - y0) - (y1 - y0)*(x2 - x0);

		if (a == 0.0f && b == 0.0f)
			return { Line2IntersectResultType::Collinear, { NAN, NAN }, NAN, NAN };

		if (b == 0.0f)
			return { Line2IntersectResultType::Parallel, { NAN, NAN }, NAN, NAN };

		const auto alpha = a / b;
		const auto beta = c / b;

		const glm::vec2 crossPos = Lerp(alpha);

		if (alpha >= 0.0f && alpha <= 1.0f && beta >= 0.0f && beta <= 1.0f)
			return { Line2IntersectResultType::SegmentIntersect, crossPos, alpha, beta };

		return { Line2IntersectResultType::LineIntersect, crossPos, alpha, beta };
	}

	constexpr glm::vec2 Lerp(float t) const
	{
		return glm::mix(p0, p1, t);
	}

	constexpr glm::vec2 Center() const
	{
		return (p0 + p1) / 3.0f;
	}

	constexpr bool Empty() const
	{
		return p0 == p1;
	}
};

struct Triangle2
{
	glm::vec2 p0, p1, p2;

	constexpr std::array<Line2, 3> Edges() const
	{
		return { Line2{ p0, p1 }, Line2{ p1, p2 }, Line2{ p2, p0 } };
	}

	constexpr glm::vec2 Center() const
	{
		return (p0 + p1 + p2) / 3.0f;
	}
};

struct Color
{
	uint8_t r, g, b, a;

	static constinit const Color WHITE;
	static constinit const Color BLACK;
	static constinit const Color TRANSPARENT;

	static constinit const Color RED;
	static constinit const Color GREEN;
	static constinit const Color BLUE;

	static constinit const Color YELLOW;
	static constinit const Color MAGENTA;
	static constinit const Color CYAN;
};

constinit inline const Color Color::WHITE{ 255, 255, 255, 255 }; // r=1, g=1, b=1, a=1
constinit inline const Color Color::BLACK{ 0, 0, 0, 255 }; // r=0, g=0 b=0, a=1
constinit inline const Color Color::TRANSPARENT{ 0, 0, 0, 0 }; // r=0, g=0, b=0, a=0

constinit inline const Color Color::RED{ 255, 0, 0, 255 }; // r=1, g=0, b=0, a=1
constinit inline const Color Color::GREEN{ 0, 255, 0, 255 }; // r=0, g=1, b=0, a=1
constinit inline const Color Color::BLUE{ 0, 0, 255, 255 }; // r=0, g=0, b=1, a=1

constinit inline const Color Color::YELLOW{ 255, 255, 0, 255 }; // r=1, g=1, b=0, a=1
constinit inline const Color Color::MAGENTA{ 255, 0, 255, 255 }; // r=1, g=0, b=1, a=1
constinit inline const Color Color::CYAN{ 0, 255, 255, 255 }; // r=0, g=1, b=1, a=1

struct ColorF
{
	float r, g, b, a;

	static constinit const ColorF WHITE;
	static constinit const ColorF BLACK;
	static constinit const ColorF TRANSPARENT;

	static constinit const ColorF RED;
	static constinit const ColorF GREEN;
	static constinit const ColorF BLUE;

	static constinit const ColorF YELLOW;
	static constinit const ColorF MAGENTA;
	static constinit const ColorF CYAN;
};

constinit inline const ColorF ColorF::WHITE{ 1.0f, 1.0f, 1.0f, 1.0f }; // r=1, g=1, b=1, a=1
constinit inline const ColorF ColorF::BLACK{ 0.0f, 0.0f, 0.0f, 1.0f }; // r=0, g=0 b=0, a=1
constinit inline const ColorF ColorF::TRANSPARENT{ 0.0f, 0.0f, 0.0f, 0.0f }; // r=0, g=0, b=0, a=0

constinit inline const ColorF ColorF::RED{ 1.0f, 0.0f, 0.0f, 1.0f }; // r=1, g=0, b=0, a=1
constinit inline const ColorF ColorF::GREEN{ 0.0f, 1.0f, 0.0f, 1.0f }; // r=0, g=1, b=0, a=1
constinit inline const ColorF ColorF::BLUE{ 0.0f, 0.0f, 1.0f, 1.0f }; // r=0, g=0, b=1, a=1

constinit inline const ColorF ColorF::YELLOW{ 1.0f, 1.0f, 0.0f, 1.0f }; // r=1, g=1, b=0, a=1
constinit inline const ColorF ColorF::MAGENTA{ 1.0f, 0.0f, 1.0f, 1.0f }; // r=1, g=0, b=1, a=1
constinit inline const ColorF ColorF::CYAN{ 0.0f, 1.0f, 1.0f, 1.0f }; // r=0, g=1, b=1, a=1

