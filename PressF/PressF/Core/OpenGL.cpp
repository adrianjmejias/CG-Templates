#include "PressF/pch.h"
#include "OpenGL.h"


GPUObject::operator unsigned int&() {
	return id;
}


void VertexBufferObject::Bind()
{
	__debugbreak();
	//glBindBuffer(id);
}

VertexBufferObject::~VertexBufferObject()
{
	if (id)
	{
		glDeleteBuffers(1, &id);
	}
}


void VertexArrayObject::Bind()
{
	glBindVertexArray(id);
}

VertexArrayObject::~VertexArrayObject()
{
	if (id)
	{
		glDeleteVertexArrays(1, &id);
	}
}


void RenderBuffer::Bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, id);

}