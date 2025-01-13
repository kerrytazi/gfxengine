#pragma once

#include "gfxengine/math.hpp"
#include "gfxengine/image.hpp"
#include "gfxengine/material.hpp"

#include <cstdint>
#include <cstddef>
#include <vector>
#include <ranges>
#include <utility>
#include <limits>
#include <functional>
#include <memory>
#include <variant>

struct FrameCacheGraphicsCache
{
	virtual ~FrameCacheGraphicsCache() = default;
};

struct DrawTaskTypes
{
	struct DrawMaterial
	{
		std::shared_ptr<Material> material;
		std::vector<uint8_t> vertices;
		std::vector<uint32_t> indices;
	};

	struct ClearBackground
	{
		ColorF color;
	};

	struct SettingWireFrame
	{
		bool enable;
	};

	struct SettingCulling
	{
		bool enable;
	};

	struct SettingBlend
	{
		bool enable;
	};

	struct SettingDepth
	{
		bool enable;
	};
};

using DrawTask = std::variant<
	DrawTaskTypes::DrawMaterial,
	DrawTaskTypes::ClearBackground,
	DrawTaskTypes::SettingWireFrame,
	DrawTaskTypes::SettingCulling,
	DrawTaskTypes::SettingBlend,
	DrawTaskTypes::SettingDepth>;

struct FrameStats
{
	size_t api_calls;
	size_t draw_calls;
	size_t vertices;
	size_t indices;
	size_t cache_vertices;
	size_t cache_indices;
};

class Frame
{
public:

	std::vector<DrawTask> tasks;

#if GFXENGINE_EDITOR
	std::function<void()> draw_editor{};
#endif // GFXENGINE_EDITOR

	void add_vertices(std::shared_ptr<Material> const &material, std::span<const uint8_t> vertices, std::span<const uint32_t> indices);

public:

	template <typename TVertex> requires(std::is_trivially_destructible_v<TVertex>)
	void add_vertices(std::shared_ptr<Material> const &material, std::span<const TVertex> vertices, std::span<const uint32_t> indices)
	{
		add_vertices(material, std::span<const uint8_t>((uint8_t const *)&*vertices.begin(), (uint8_t const *)&*vertices.end()), indices);
	}

	template <typename TVertex> requires(std::is_trivially_destructible_v<TVertex>)
	void add_quad(std::shared_ptr<Material> const &material, TVertex const &v0, TVertex const &v1, TVertex const &v2, TVertex const &v3)
	{
		add_vertices<TVertex>(
			material,
			std::initializer_list<TVertex>{ v0, v1, v2, v3 },
			std::initializer_list<uint32_t>{ 0, 1, 2, 0, 2, 3, }
		);
	}

	void clear_background(ColorF const &color)
	{
		tasks.push_back(DrawTask(DrawTaskTypes::ClearBackground{ .color = color }));
	}

	void setting_wireframe(bool enable)
	{
		tasks.push_back(DrawTask(DrawTaskTypes::SettingWireFrame{ .enable = enable }));
	}

	void setting_culling(bool enable)
	{
		tasks.push_back(DrawTask(DrawTaskTypes::SettingCulling{ .enable = enable }));
	}

	void setting_blend(bool enable)
	{
		tasks.push_back(DrawTask(DrawTaskTypes::SettingBlend{ .enable = enable }));
	}

	void setting_depth(bool enable)
	{
		tasks.push_back(DrawTask(DrawTaskTypes::SettingDepth{ .enable = enable }));
	}

#if GFXENGINE_EDITOR
	void on_draw_editor(std::function<void()> on_draw)
	{
		draw_editor = std::move(on_draw);
	}
#endif // GFXENGINE_EDITOR

	void reset()
	{
		tasks.clear();

#if GFXENGINE_EDITOR
		draw_editor = {};
#endif // GFXENGINE_EDITOR
	}

	FrameStats get_stats() const;
};
