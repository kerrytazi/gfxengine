#pragma once

class FrameCache;
class Frame;

class Graphics
{
public:

	virtual ~Graphics() = default;

	virtual void draw(Frame const &frame) = 0;
	virtual void cache(FrameCache &cache) = 0;
};
