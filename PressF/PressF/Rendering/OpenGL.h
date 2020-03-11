#pragma once
#include <glad/glad.h>

using glId = unsigned int;

struct GPUObject
{
	glId id = 0;
	operator unsigned int&() {
		return id;
	}
};

struct VertexArrayObject : GPUObject{
	~VertexArrayObject()
	{
		if (id)
		{
			glDeleteVertexArrays(1, &id);
		}
	}
};

struct VertexBufferObject : GPUObject{
	~VertexBufferObject()
	{
		if (id)
		{
			glDeleteBuffers(1, &id);
		}
	}
};


