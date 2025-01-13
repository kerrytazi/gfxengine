#pragma once

#include <memory>

struct FrameCacheGraphicsCache;
struct FrameCache;
class Frame;
struct CreateMaterialParams;
struct Material;

class Graphics
{
public:

	virtual ~Graphics() = default;

	virtual void draw(Frame const &frame) = 0;
	virtual std::shared_ptr<Material> create_material(CreateMaterialParams const &params) = 0;
};
