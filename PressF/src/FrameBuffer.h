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
	static std::unique_ptr<FrameBuffer> GetShadowBuffer(int width, int heigth) {
		std::unique_ptr<FrameBuffer> fb(new FrameBuffer(width, heigth));
		Texture &texture=  fb->texture;
		texture.width = width;
		texture.height = heigth;

		glGenFramebuffers(1, &fb->id);

		glGenTextures(1, &texture.id);
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			width, heigth, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindFramebuffer(GL_FRAMEBUFFER, fb->id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.id, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return fb;
	}
	static std::unique_ptr<FrameBuffer> GetGBuffer(int width, int heigth)
	{
		std::unique_ptr<FrameBuffer> fb(new FrameBuffer(width, heigth));
		Texture &texture = fb->texture;
		texture.width = width;
		texture.height = heigth;

		glGenFramebuffers(1, &fb->id);

		glGenTextures(1, &texture.id);
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, heigth, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, fb->id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return fb;
	}
	~FrameBuffer();
};

