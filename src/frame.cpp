#include "gfxengine/frame.hpp"

void FrameCache::add_vertices(std::span<const Vertex> _vertices, std::span<const uint32_t> _indices)
{
	const uint32_t offset = vertices.size();

	vertices.insert(vertices.end(), _vertices.begin(), _vertices.end());

	indices.insert(indices.end(), _indices.begin(), _indices.end());
	for (auto i = indices.size() - _indices.size(); i != indices.size(); ++i)
		indices[i] += offset;
}

void Frame::add_vertices(std::span<const Vertex> _vertices, std::span<const uint32_t> _indices, std::weak_ptr<Image> _img /*= {}*/)
{
	bool batched = false;

	if (!tasks.empty())
	{
		if (auto content = std::get_if<DrawTaskTypes::DrawIndices>(&tasks.back()))
		{
			if (content->texture.lock() == _img.lock())
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
