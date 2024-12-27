#pragma once

#include "gfxengine/math.hpp"

#include <cstdint>
#include <cstddef>
#include <vector>
#include <ranges>
#include <utility>
#include <limits>
#include <functional>


struct Vertex
{
	glm::vec3 pos;
	Color color;
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

struct DrawTask
{
	enum class Type
	{
		DrawIndices,
		UpdateMVP,
		ClearBackground,
		SettingWireFrame,
		SettingCulling,
		SettingBlend,
		SettingDepth,
	};

	union Content
	{
		struct DrawIndices
		{
			size_t from;
			size_t count;
		} draw_indices;

		struct UpdateMVP
		{
			glm::mat4 mvp;
		} update_mvp;

		struct ClearBackground
		{
			Color color;
		} clear_background;

		struct SettingWireFrame
		{
			bool enable;
		} setting_wireframe;

		struct SettingCulling
		{
			bool enable;
		} setting_culling;

		struct SettingBlend
		{
			bool enable;
		} setting_blend;

		struct SettingDepth
		{
			bool enable;
		} setting_depth;
	};

	Type type;
	Content content;

	DrawTask(Content::DrawIndices draw_indices)
	{
		type = Type::DrawIndices;
		new (&content.draw_indices) Content::DrawIndices{ draw_indices };
	}

	DrawTask(Content::UpdateMVP update_mvp)
	{
		type = Type::UpdateMVP;
		new (&content.update_mvp) Content::UpdateMVP{ update_mvp };
	}

	DrawTask(Content::ClearBackground clear_background)
	{
		type = Type::ClearBackground;
		new (&content.clear_background) Content::ClearBackground{ clear_background };
	}

	DrawTask(Content::SettingWireFrame setting_wireframe)
	{
		type = Type::SettingWireFrame;
		new (&content.setting_wireframe) Content::SettingWireFrame{ setting_wireframe };
	}

	DrawTask(Content::SettingCulling setting_culling)
	{
		type = Type::SettingCulling;
		new (&content.setting_culling) Content::SettingCulling{ setting_culling };
	}

	DrawTask(Content::SettingBlend setting_blend)
	{
		type = Type::SettingBlend;
		new (&content.setting_blend) Content::SettingBlend{ setting_blend };
	}

	DrawTask(Content::SettingDepth setting_depth)
	{
		type = Type::SettingDepth;
		new (&content.setting_depth) Content::SettingDepth{ setting_depth };
	}

	~DrawTask()
	{
		switch (type)
		{
			case Type::DrawIndices:
				content.draw_indices.~DrawIndices();
				break;
			case Type::UpdateMVP:
				content.update_mvp.~UpdateMVP();
				break;
			case Type::ClearBackground:
				content.clear_background.~ClearBackground();
				break;
			case Type::SettingWireFrame:
				content.setting_wireframe.~SettingWireFrame();
				break;
			case Type::SettingCulling:
				content.setting_culling.~SettingCulling();
				break;
			case Type::SettingBlend:
				content.setting_blend.~SettingBlend();
				break;
			case Type::SettingDepth:
				content.setting_depth.~SettingDepth();
				break;
		}
	}
};

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
};

class Frame
{
public:

	FrameCache data = FrameCache::with_reserve(64 * 1024);

	std::vector<DrawTask> tasks;
	std::vector<FrameCache *> caches;

	glm::mat4 model = glm::identity<glm::mat4>();
	glm::mat4 view = glm::identity<glm::mat4>();
	glm::mat4 projection = glm::identity<glm::mat4>();

#if GFXENGINE_EDITOR
	std::function<void()> draw_editor{};
#endif // GFXENGINE_EDITOR

public:

	void add_vertices(std::span<const Vertex> _vertices, std::span<const uint32_t> _indices);

	void add_triangle(Vertex const &v0, Vertex const &v1, Vertex const &v2)
	{
		add_vertices(
			std::initializer_list<Vertex>{ v0, v1, v2 },
			std::initializer_list<uint32_t>{ 0, 1, 2 }
		);
	}

	void add_triangle(glm::vec2 const &pos0, glm::vec2 const &pos1, glm::vec2 const &pos2, Color color = Color::WHITE)
	{
		add_triangle(Vertex{ glm::vec3(pos0, 0.0f), color }, { glm::vec3(pos1, 0.0f), color }, { glm::vec3(pos2, 0.0f), color });
	}

	void add_quad(Vertex const &v0, Vertex const &v1, Vertex const &v2, Vertex const &v3)
	{
		add_vertices(
			std::initializer_list<Vertex>{ v0, v1, v2, v3 },
			std::initializer_list<uint32_t>{ 0, 1, 2, 0, 2, 3, }
		);
	}

	void add_quad(glm::vec3 const &pos0, glm::vec3 const &pos1, glm::vec3 const &pos2, glm::vec3 const &pos3, Color color = Color::WHITE)
	{
		add_quad(Vertex{ pos0, color }, { pos1, color }, { pos2, color }, { pos3, color });
	}

	void add_quad(glm::vec2 const &pos0, glm::vec2 const &pos1, glm::vec2 const &pos2, glm::vec2 const &pos3, Color color = Color::WHITE)
	{
		add_quad(Vertex{ glm::vec3(pos0, 0.0f), color }, { glm::vec3(pos1, 0.0f), color }, { glm::vec3(pos2, 0.0f), color }, { glm::vec3(pos3, 0.0f), color });
	}

	void clear_background(Color const &color)
	{
		// TODO: batch
		tasks.push_back(DrawTask(DrawTask::Content::ClearBackground{ .color = color }));
	}

	void on_mvp_update()
	{
		// TODO: batch
		tasks.push_back(DrawTask(DrawTask::Content::UpdateMVP{ .mvp = projection * view * model }));
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
		tasks.push_back(DrawTask(DrawTask::Content::SettingWireFrame{ .enable = enable }));
	}

	void setting_culling(bool enable)
	{
		tasks.push_back(DrawTask(DrawTask::Content::SettingCulling{ .enable = enable }));
	}

	void setting_blend(bool enable)
	{
		tasks.push_back(DrawTask(DrawTask::Content::SettingBlend{ .enable = enable }));
	}

	void setting_depth(bool enable)
	{
		tasks.push_back(DrawTask(DrawTask::Content::SettingDepth{ .enable = enable }));
	}

#if GFXENGINE_EDITOR
	void on_draw_editor(std::function<void()> on_draw)
	{
		draw_editor = std::move(on_draw);
	}
#endif // GFXENGINE_EDITOR

	FrameCache cached(auto func)
	{
		FrameCache c;
		caches.push_back(&c);
		func();
		caches.pop_back();
		return c;
	}

	void add_cached(FrameCache const &cache)
	{
		add_vertices(cache.vertices, cache.indices);
	}
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
