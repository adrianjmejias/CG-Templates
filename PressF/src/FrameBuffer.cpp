#include "types.h"
#include "FrameBuffer.h"



FrameBuffer::FrameBuffer(int _width, int _height) 
{
	this->texture.width = _width;
	this->texture.height = _height;
	
}


FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &id);
}
