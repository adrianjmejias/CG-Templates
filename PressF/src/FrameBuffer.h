#pragma once
#include "core.h"


enum class FrameBufferType {
	DEPTH,
	FULL,
	STENCIL
};


class FrameBuffer
{
public:
	unsigned int id = 0;
	FrameBufferType type = FrameBufferType::DEPTH;
	Texture texture;
	FrameBuffer(int width, int height);
	~FrameBuffer();
};

