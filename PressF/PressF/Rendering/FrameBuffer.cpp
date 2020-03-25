#include "PressF/pch.h"
#include "FrameBuffer.h"


namespace PF
{

	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);

	}

	void FrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	Texture* FrameBuffer::AddColorAttachment()
	{
		colors.emplace_back();
		return &colors[colors.size() -1];
	}

	RenderBuffer* FrameBuffer::AddDepthAttachment()
	{
		depthBuffers.emplace_back();
		return  &depthBuffers[depthBuffers.size() - 1];
	}

	void FrameBuffer::Clear()
	{
		colors.clear();
		depthBuffers.clear();
	}

	void FrameBuffer::SetSize(int nWidth, int nHeight)
	{
		width = nWidth;
		height = nHeight;

		for (auto& c : colors)
		{
			*c.width = width;
			*c.height = height;
		}

		GPUInstantiate();
		
	}

	void FrameBuffer::Generate()
	{
		if (id)
		{
			glDeleteFramebuffers(1, &id);
		}

		glGenFramebuffers(1, &id);
	}

	void FrameBuffer::AttachColorBuffers()
	{
		attachmentIds.clear();
		attachmentIds.reserve(colors.size());

		for (int ii = 0; ii < colors.size(); ii++)
		{
			auto& color = colors[ii];
			color.GPUInstantiate();
			GLenum	 attachmentId = GL_COLOR_ATTACHMENT0 + ii;
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentId, static_cast<int>(color.texType), color.id, 0);

			attachmentIds.push_back(attachmentId);
		}
		glDrawBuffers((GLenum)attachmentIds.size(), attachmentIds.data());
	}

	void FrameBuffer::AttachDepthBuffer()
	{

		for (int ii = 0; ii < depthBuffers.size(); ii++)
		{
			auto& depth = depthBuffers[ii];
			depth.Generate();
			glBindRenderbuffer(GL_RENDERBUFFER, depth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
		}
	}

	void FrameBuffer::GPUInstantiate()
	{
		Generate();
		Bind();
		{
			AttachColorBuffers();
			AttachDepthBuffer();
			CheckErrors();
		}
		UnBind();
	}

	bool FrameBuffer::CheckErrors()
	{
		bool error = glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE;

		if (error)
		{
			__debugbreak();
			std::cout << "Framebuffer not complete!" << std::endl;
		}

		return error;
	}

}
