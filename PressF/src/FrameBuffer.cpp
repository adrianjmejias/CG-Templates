#include "FrameBuffer.h"



FrameBuffer::FrameBuffer(int width, int height)
{
	glCreateFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	texture.width = width;
	texture.height = height;


	switch (type)
	{
	case FrameBufferType::DEPTH:
		glGenTextures(1, &texture.id);
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
			GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
		);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, id, 0);
		break;
	case FrameBufferType::FULL:
		break;
	case FrameBufferType::STENCIL:
		break;
	default:
		break;
	}


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		PF_ASSERT(false && "something happened");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
		PF_ASSERT(false && "something happened");
	
}


FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &id);
}
