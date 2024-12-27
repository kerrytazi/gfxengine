#pragma once

class Frame;

class BaseGraphics
{
public:

	virtual ~BaseGraphics() = default;

	virtual void draw(Frame const &frame) = 0;
};
