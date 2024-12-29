#pragma once

#include "gfxengine/math.hpp"
#include "gfxengine/image.hpp"

#include <cstdint>
#include <cstddef>
#include <vector>
#include <ranges>
#include <utility>
#include <limits>
#include <functional>
#include <memory>
#include <variant>


struct Vertex
{
	glm::vec3 pos;
	Color color;
	glm::vec2 tex_coord;
	glm::vec2 tex_offset;
};

struct Frame;

struct MatrixGuard
{
	Frame &frame;
	glm::mat4 *matrix;
	glm::mat4 save_matrix;

	MatrixGuard(MatrixGuard const &) = delete;
	MatrixGuard &operator = (MatrixGuard const &) = delete;

	MatrixGuard(MatrixGuard &&other) noexcept
		: frame{ other.frame }
		, matrix{ other. matrix }
		, save_matrix{ {} }
	{
		other.matrix = nullptr;
	}

	MatrixGuard &operator = (MatrixGuard &&other) noexcept
	{
		this->~MatrixGuard();
		new (this) MatrixGuard(std::move(other));
		return *this;
	}

	MatrixGuard(Frame &_frame, glm::mat4 *_matrix);
	~MatrixGuard();
};

struct FrameCacheGraphicsCache
{
	virtual ~FrameCacheGraphicsCache() = default;
};

struct DrawTaskTypes
{
	struct DrawIndices
	{
		size_t from;
		size_t count;
		std::shared_ptr<Image> texture;
	};

	struct DrawCached
	{
		std::shared_ptr<FrameCacheGraphicsCache> graphics_cache;
		std::shared_ptr<Image> texture;
		size_t stat_vertices;
		size_t stat_indices;
	};

	struct UpdateMVP
	{
		glm::mat4 mvp;
	};

	struct ClearBackground
	{
		Color color;
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
	DrawTaskTypes::DrawIndices,
	DrawTaskTypes::DrawCached,
	DrawTaskTypes::UpdateMVP,
	DrawTaskTypes::ClearBackground,
	DrawTaskTypes::SettingWireFrame,
	DrawTaskTypes::SettingCulling,
	DrawTaskTypes::SettingBlend,
	DrawTaskTypes::SettingDepth>;

struct FrameCache
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	static FrameCache with_reserve(size_t count)
	{
		FrameCache result;
		result.vertices.reserve(count);
		result.indices.reserve(count);
		return result;
	}

	void add_vertices(std::span<const Vertex> _vertices, std::span<const uint32_t> _indices);

	bool empty() const
	{
		return vertices.empty();
	}

	void clear()
	{
		vertices.clear();
		indices.clear();
	}
};

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

	FrameCache data = FrameCache::with_reserve(1024 * 1024);

	std::vector<DrawTask> tasks;
	std::vector<FrameCache *> caches;

	glm::mat4 model = glm::identity<glm::mat4>();
	glm::mat4 view = glm::identity<glm::mat4>();
	glm::mat4 projection = glm::identity<glm::mat4>();

#if GFXENGINE_EDITOR
	std::function<void()> draw_editor{};
#endif // GFXENGINE_EDITOR

public:

	void add_vertices(std::span<const Vertex> _vertices, std::span<const uint32_t> _indices, std::shared_ptr<Image> _img = {});

	void add_triangle(Vertex const &v0, Vertex const &v1, Vertex const &v2, std::shared_ptr<Image> img = {})
	{
		add_vertices(
			std::initializer_list<Vertex>{ v0, v1, v2 },
			std::initializer_list<uint32_t>{ 0, 1, 2 },
			img
		);
	}

	void add_triangle(glm::vec2 const &pos0, glm::vec2 const &pos1, glm::vec2 const &pos2, Color color = Color::WHITE, std::shared_ptr<Image> img = {}, glm::vec2 const &tex0 = {}, glm::vec2 const &tex1 = {}, glm::vec2 const &tex2 = {})
	{
		add_triangle(Vertex{ glm::vec3(pos0, 0.0f), color, tex0 }, { glm::vec3(pos1, 0.0f), color, tex1 }, { glm::vec3(pos2, 0.0f), color, tex2 });
	}

	void add_quad(Vertex const &v0, Vertex const &v1, Vertex const &v2, Vertex const &v3, std::shared_ptr<Image> img = {})
	{
		add_vertices(
			std::initializer_list<Vertex>{ v0, v1, v2, v3 },
			std::initializer_list<uint32_t>{ 0, 1, 2, 0, 2, 3, },
			img
		);
	}

	void add_quad(glm::vec3 const &pos0, glm::vec3 const &pos1, glm::vec3 const &pos2, glm::vec3 const &pos3, Color color = Color::WHITE, std::shared_ptr<Image> img = {}, glm::vec2 const &tex0 = {}, glm::vec2 const &tex1 = {}, glm::vec2 const &tex2 = {}, glm::vec2 const &tex3 = {})
	{
		add_quad(Vertex{ pos0, color, tex0 }, { pos1, color, tex1 }, { pos2, color, tex2 }, { pos3, color, tex3 }, img);
	}

	void add_quad(glm::vec2 const &pos0, glm::vec2 const &pos1, glm::vec2 const &pos2, glm::vec2 const &pos3, Color color = Color::WHITE, std::shared_ptr<Image> img = {}, glm::vec2 const &tex0 = {}, glm::vec2 const &tex1 = {}, glm::vec2 const &tex2 = {}, glm::vec2 const &tex3 = {})
	{
		add_quad(Vertex{ glm::vec3(pos0, 0.0f), color, tex0 }, { glm::vec3(pos1, 0.0f), color, tex1 }, { glm::vec3(pos2, 0.0f), color, tex2 }, { glm::vec3(pos3, 0.0f), color, tex3 }, img);
	}

	void clear_background(Color const &color)
	{
		// TODO: batch
		tasks.push_back(DrawTask(DrawTaskTypes::ClearBackground{ .color = color }));
	}

	void on_mvp_update()
	{
		// TODO: batch
		tasks.push_back(DrawTask(DrawTaskTypes::UpdateMVP{ .mvp = projection * view * model }));
	}

	MatrixGuard set_model_matrix(glm::mat4 const &_model)
	{
		// TODO: batch
		if (_model == model)
			return MatrixGuard(*this, nullptr);

		MatrixGuard guard(*this, &model);
		model = _model;
		on_mvp_update();
		return guard;
	}

	MatrixGuard set_view_matrix(glm::mat4 const &_view)
	{
		// TODO: batch
		if (_view == view)
			return MatrixGuard(*this, nullptr);

		MatrixGuard guard(*this, &view);
		view = _view;
		on_mvp_update();
		return guard;
	}

	MatrixGuard set_projection_matrix(glm::mat4 const &_projection)
	{
		// TODO: batch
		if (_projection == projection)
			return MatrixGuard(*this, nullptr);

		MatrixGuard guard(*this, &projection);
		projection = _projection;
		on_mvp_update();
		return guard;
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

	void cached(FrameCache &cache, auto func)
	{
		caches.push_back(&cache);
		func();
		caches.pop_back();
	}

	void add_cached(FrameCache const &cache, std::shared_ptr<FrameCacheGraphicsCache> const &graphics_cache, std::shared_ptr<Image> _img = {})
	{
		if (graphics_cache)
		{
			tasks.push_back(DrawTask(DrawTaskTypes::DrawCached{
				.graphics_cache = graphics_cache,
				.texture = _img,
				.stat_vertices = cache.vertices.size(),
				.stat_indices = cache.indices.size(),
			}));
		}
		else
		{
			add_vertices(cache.vertices, cache.indices, _img);
		}
	}

	void clear()
	{
		data.clear();
		tasks.clear();

#if GFXENGINE_EDITOR
	draw_editor = {};
#endif // GFXENGINE_EDITOR
	}

	FrameStats get_stats() const;
};

inline MatrixGuard::MatrixGuard(Frame &_frame, glm::mat4 *_matrix)
	: frame{ _frame }
	, matrix{ _matrix }
	, save_matrix{ {} }
{
	if (matrix)
		save_matrix = *_matrix;
}

inline MatrixGuard::~MatrixGuard()
{
	if (matrix)
	{
		*matrix = save_matrix;
		frame.on_mvp_update();
	}
}
