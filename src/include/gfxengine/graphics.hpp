#pragma once

#include "gfxengine/math.hpp"

#include <memory>

struct GraphicsCacheVertices;
class Frame;
struct CreateMaterialParams;
struct Material;

class Graphics
{
public:

	virtual ~Graphics() = default;

	virtual void draw(Frame const &frame) = 0;
	virtual std::shared_ptr<Material> create_material(CreateMaterialParams const &params) = 0;
	virtual std::shared_ptr<GraphicsCacheVertices> create_cache_vertices(std::shared_ptr<Material> material) = 0;
	virtual void resize(ivec2 size, float resolution_scale) = 0;
};
