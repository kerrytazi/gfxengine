#include "gfxengine/frame.hpp"

void FrameCache::add_vertices(std::span<const Vertex> _vertices, std::span<const uint32_t> _indices)
{
	const uint32_t offset = vertices.size();

	vertices.insert(vertices.end(), _vertices.begin(), _vertices.end());

	indices.insert(indices.end(), _indices.begin(), _indices.end());
	for (auto i = indices.size() - _indices.size(); i != indices.size(); ++i)
		indices[i] += offset;
}

void Frame::add_vertices(std::span<const Vertex> _vertices, std::span<const uint32_t> _indices, std::shared_ptr<Image> _img /*= {}*/)
{
	bool batched = false;

	if (!tasks.empty())
	{
		if (auto content = std::get_if<DrawTaskTypes::DrawIndices>(&tasks.back()))
		{
			if (content->texture == _img)
			{
				content->count += _indices.size();
				batched = true;
			}
		}
	}

	if (!batched)
	{
		tasks.push_back(DrawTask(DrawTaskTypes::DrawIndices{ .from = data.indices.size(), .count = _indices.size(), .texture = _img }));
	}

	data.add_vertices(_vertices, _indices);

	for (auto cache : caches)
		cache->add_vertices(_vertices, _indices);
}

FrameStats Frame::get_stats() const
{
	FrameStats result{};

	result.vertices = data.vertices.size();
	result.indices = data.indices.size();

	for (auto const &task : tasks)
	{
		std::visit([&](auto const &content) {
			using T = std::decay_t<decltype(content)>;

			if constexpr (std::is_same_v<T, DrawTaskTypes::DrawIndices>)
			{
				result.draw_calls += 1;
			}
			else
			if constexpr (std::is_same_v<T, DrawTaskTypes::DrawCached>)
			{
				result.draw_calls += 1;
				result.cache_vertices += content.stat_vertices;
				result.cache_indices += content.stat_indices;
			}
			else
			{
				result.api_calls += 1;
			}

		}, task);
	}

	return result;
}
