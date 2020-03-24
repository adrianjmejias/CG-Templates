#include "PressF/pch.h"
#include "FrameBuffer.h"


namespace PF
{
	FrameBuffer::FrameBuffer(int width, int height)
	{
        this->width = width;
        this->heigth = height;

       // glGenFramebuffers(1, &id);
       // Bind();
       // rbo.reset(new RenderBuffer());
       // pos.reset(new Texture(TexColorFormat::RGB, TexColorFormat::RGB16F, TexPixelType::FLOAT, width, height));
       //normal.reset(new Texture(TexColorFormat::RGB, TexColorFormat::RGB16F, TexPixelType::FLOAT, width, height)); 
       //color.reset(new Texture(TexColorFormat::RGBA, TexColorFormat::RGBA, TexPixelType::UNSIGNED_BYTE, width, height));

       // pos->Bind();
       // pos->SetInterpolationMethod(TexInterpolationMethod::NEAREST, TexInterpolationMethod::NEAREST);
       // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *pos, 0);
       // 
       // // - normal color buffer
       // normal->Bind();
       // normal->SetInterpolationMethod(TexInterpolationMethod::NEAREST, TexInterpolationMethod::NEAREST);
       // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, *normal, 0);


       // // - color + specular color buffer
       // color->Bind();
       // color->SetInterpolationMethod(TexInterpolationMethod::NEAREST, TexInterpolationMethod::NEAREST);
       // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, *color, 0);

       // // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
       // unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
       // glDrawBuffers(3, attachments);


       // 
       // Texture dpth(TexColorFormat::RGBA, TexColorFormat::RGBA, TexPixelType::UNSIGNED_BYTE, width, height);
       // // create and attach depth buffer (renderbuffer)
       // glGenRenderbuffers(1, &rbo->id);
       // glBindRenderbuffer(GL_RENDERBUFFER, rbo->id);
       // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
       // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo->id);



       // // finally check if framebuffer is complete
       // if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
       //     std::cout << "Framebuffer not complete!" << std::endl;
       // 


       // UnBind();

        rbo.reset(new RenderBuffer());
        pos.reset(new Texture());
        normal.reset(new Texture());
        color.reset(new Texture());

        glGenFramebuffers(1, &id);
        glBindFramebuffer(GL_FRAMEBUFFER, id);
        unsigned int &gPosition = pos->id;
        unsigned int &gNormal = normal->id;
        unsigned int &gAlbedoSpec = color->id;
        // position color buffer
        glGenTextures(1, &gPosition);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
        // normal color buffer
        glGenTextures(1, &gNormal);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
        // color + specular color buffer
        glGenTextures(1, &gAlbedoSpec);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
        // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
        unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(3, attachments);
        // create and attach depth buffer (renderbuffer)

        
        unsigned int &rboDepth = rbo->id;
        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
        // finally check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);














        //glGenFramebuffers(1, &id);
        //Bind();
        //// create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)
        //for (unsigned int i = 0; i < 2; i++)
        //{
        //    Texture t;
        //    glGenTextures(1, &t.id);
        //    t.Bind();
        //    {
        //        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, width, 0, GL_RGB, GL_FLOAT, NULL);
        //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //        // attach texture to framebuffer
        //        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, t, 0);
        //    }

        //    texs.push_back(t);
        //}

        //// create and attach depth buffer (renderbuffer)
        //Texture t;
        //glGenRenderbuffers(1, &rbo.id);
        //rbo.Bind();
        //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

        //// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
        //unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        //glDrawBuffers(2, attachments);
        //// finally check if framebuffer is complete
        //if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        //    std::cout << "Framebuffer not complete!" << std::endl;

        //UnBind();
















        //// ping-pong-framebuffer for blurring
        //unsigned int pingpongFBO[2];
        //unsigned int pingpongColorbuffers[2];
        //glGenFramebuffers(2, pingpongFBO);
        //glGenTextures(2, pingpongColorbuffers);
        //for (unsigned int i = 0; i < 2; i++)
        //{
        //    glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        //    glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GL_COLOR_ATTACHMENT0+1, 0);
        //    // also check if framebuffers are complete (no need for depth buffer)
        //    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        //        std::cout << "Framebuffer not complete!" << std::endl;
        //}




	}



	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);

	}

	void FrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
