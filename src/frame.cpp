#include "gfxengine/frame.hpp"

void Frame::add_vertices(std::shared_ptr<Material> const &material, std::span<const uint8_t> vertices, std::span<const uint32_t> indices)
{
	if (!tasks.empty())
	{
		if (DrawTaskTypes::DrawMaterial *prev = std::get_if<DrawTaskTypes::DrawMaterial>(&tasks.back()))
		{
			if (prev->material == material)
			{
				size_t prev_vertices = prev->vertices.size() / material->attribute_info.total_byte_size;
				size_t prev_indices = prev->indices.size();

				prev->vertices.insert(prev->vertices.end(), vertices.begin(), vertices.end());
				prev->indices.insert(prev->indices.end(), indices.begin(), indices.end());

				size_t new_indices = prev->indices.size();

				for (size_t i = prev_indices; i != new_indices; ++i)
					prev->indices[i] += (uint32_t)prev_vertices;

				return;
			}
		}
	}

	tasks.push_back(DrawTask(DrawTaskTypes::DrawMaterial{
		.material = material,
		.vertices = std::vector<uint8_t>(vertices.begin(), vertices.end()),
		.indices = std::vector<uint32_t>(indices.begin(), indices.end()),
	}));
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
			{
				result.api_calls += 1;
			}

		}, task);
	}

	return result;
}
