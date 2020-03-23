#pragma once


#ifndef __ED_QUAD__
#define __ED_QUAD__


#include <glad/glad.h>

namespace PF
{
	class Quad {
		static Quad* instance;
		VertexBufferObject VBO;
		VertexArrayObject VAO;
	public:
		Quad();
		~Quad();
		void Draw();
		void Bind();
		static Quad* Instance();
	};
}

#endif // !__ED_QUAD__

