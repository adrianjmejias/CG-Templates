#pragma once
#include <glad/glad.h>

using glId = unsigned int;

struct GPUObject
{
	glId id = 0;
	operator unsigned int& ();
	virtual void Bind() = 0;
};

struct VertexArrayObject : GPUObject{

	virtual void Bind() override;
	
	~VertexArrayObject();
};

struct VertexBufferObject : GPUObject{
	//glId type;
	virtual void Bind() override;
	~VertexBufferObject();
};

struct RenderBuffer : GPUObject
{
	int width, height;
	// Inherited via GPUObject
	virtual void Bind() override;
	void Reset(unsigned int nId = 0)
	{
		if (id)
		{
			glDeleteRenderbuffers(1, &id);
		}
		id = nId;
	}
	~RenderBuffer()
	{
		Reset();
	}

	void Generate()
	{

		if (id)
		{
			glDeleteRenderbuffers(1, &id);
		}

		glGenRenderbuffers(1, &id);

	}


};


