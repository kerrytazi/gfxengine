#include "gfxengine/frame.hpp"

static void copy_vertices_adjusted(std::shared_ptr<Material> const &material, std::vector<uint8_t> &vertices, std::vector<uint32_t> &indices, std::span<const uint8_t> _vertices, std::span<const uint32_t> _indices)
{
	size_t prev_vertices = vertices.size() / material->attribute_info.total_byte_size;
	size_t prev_indices = indices.size();

	vertices.insert(vertices.end(), _vertices.begin(), _vertices.end());
	indices.insert(indices.end(), _indices.begin(), _indices.end());

	size_t new_indices = indices.size();

	for (size_t i = prev_indices; i != new_indices; ++i)
		indices[i] += (uint32_t)prev_vertices;
}

void FrameCacheVertices::add_vertices(std::shared_ptr<Material> const &material, std::span<const uint8_t> _vertices, std::span<const uint32_t> _indices)
{
	// TODO: assert same material attributes
	copy_vertices_adjusted(material, vertices, indices, _vertices, _indices);
}

void Frame::add_vertices(std::shared_ptr<Material> const &material, std::span<const uint8_t> _vertices, std::span<const uint32_t> _indices)
{
	if (!caches.empty())
	{
		for (FrameCacheVertices *c : caches)
			c->add_vertices(material, _vertices, _indices);

		return;
	}

	if (!tasks.empty())
	{
		if (DrawTaskTypes::DrawMaterial *prev = std::get_if<DrawTaskTypes::DrawMaterial>(&tasks.back()))
		{
			if (prev->material == material)
			{
				copy_vertices_adjusted(material, prev->vertices, prev->indices, _vertices, _indices);
				return;
			}
		}
	}

	tasks.push_back(DrawTask(DrawTaskTypes::DrawMaterial{
		.material = material,
		.vertices = std::vector<uint8_t>(_vertices.begin(), _vertices.end()),
		.indices = std::vector<uint32_t>(_indices.begin(), _indices.end()),
	}));
}

void Frame::add_cached_vertices(std::shared_ptr<Material> const &material, FrameCacheVertices const &c)
{
	add_vertices(material, c.vertices, c.indices);
}

FrameStats Frame::get_stats() const
{
	FrameStats result{};

	for (auto const &task : tasks)
	{
		std::visit([&](auto const &content) {
			using T = std::decay_t<decltype(content)>;

			if constexpr (std::is_same_v<T, DrawTaskTypes::DrawMaterial>)
			{
				result.draw_calls += 1;

				result.vertices += content.vertices.size() / content.material->attribute_info.total_byte_size;
				result.indices += content.indices.size();
			}
			else
			if constexpr (std::is_same_v<T, DrawTaskTypes::DrawCached>)
			{
				result.draw_calls += 1;

				result.cache_vertices += content.cache->stats_vertices_count;
				result.cache_indices += content.cache->stats_indices_count;
			}

		}, task);
	}

	return result;
}
