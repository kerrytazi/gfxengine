#include "gfxengine/frame.hpp"

void FrameCache::add_vertices(std::span<const Vertex> _vertices, std::span<const uint32_t> _indices)
{
	const uint32_t offset = vertices.size();

	vertices.insert(vertices.end(), _vertices.begin(), _vertices.end());

	indices.insert(indices.end(), _indices.begin(), _indices.end());
	for (auto i = indices.size() - _indices.size(); i != indices.size(); ++i)
		indices[i] += offset;
}

void Frame::add_vertices(std::span<const Vertex> _vertices, std::span<const uint32_t> _indices)
{
	if (!tasks.empty()
		&& tasks.back().type == DrawTask::Type::DrawIndices)
	{
		tasks.back().content.draw_indices.count += _indices.size();
	}
	else
	{
		tasks.push_back(DrawTask(DrawTask::Content::DrawIndices{ .from = data.indices.size(), .count = _indices.size() }));
	}

	data.add_vertices(_vertices, _indices);

	for (auto cache : caches)
		cache->add_vertices(_vertices, _indices);
}
