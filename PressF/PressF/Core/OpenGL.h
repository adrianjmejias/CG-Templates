#pragma once
#include <glad/glad.h>

using glId = unsigned int;

struct GPUObject
{
	glId id = 0;
	operator unsigned int&() {
		return id;
	}
	virtual void Bind() = 0;
};

struct VertexArrayObject : GPUObject{

	virtual void Bind() override
	{
		glBindVertexArray(id);
	}

	~VertexArrayObject()
	{
		if (id)
		{
			glDeleteVertexArrays(1, &id);
		}
	}
};

struct VertexBufferObject : GPUObject{
	glId type;
	virtual void Bind() override
	{
		__debugbreak();
		//glBindBuffer(id);
	}
	~VertexBufferObject()
	{
		if (id)
		{
			glDeleteBuffers(1, &id);
		}
	}
};


