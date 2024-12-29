#pragma once

#include <memory>

class FrameCacheGraphicsCache;
class FrameCache;
class Frame;

class Graphics
{
public:

	virtual ~Graphics() = default;

	virtual void draw(Frame const &frame) = 0;
	virtual void cache(FrameCache &cache, std::shared_ptr<FrameCacheGraphicsCache> &graphics_cache) = 0;
};
