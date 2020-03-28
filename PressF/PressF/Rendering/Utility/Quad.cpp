#include "PressF/pch.h"
#include "Quad.h"
//static Quad* instance;
//static unsigned int VBO, VAO, EBO;


namespace PF
{
	Quad* Quad::instance = nullptr;
	//unsigned int Quad::VBO = 0;
	//unsigned int Quad::VAO = 0;


	Quad::Quad() {
		const float sz = 1.f;
		float quadVertices[] = {
			// positions        	// texture Coords
			-sz,sz,
			sz, sz,
			-sz,-sz,
			sz,-sz,
		};

		// setup plane VAO
		glGenVertexArrays(1, &VAO.id);
		glGenBuffers(1, &VBO.id);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
		glBindVertexArray(0);

	}

	void Quad::Bind()
	{
		VAO.Bind();
	}

	void Quad::Draw()
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	Quad* Quad::Instance() {
		if (!instance)
		{
			instance = new Quad();
		}

		return instance;
	}
	void Quad::BindDraw()
	{
		Quad* instance = Instance();
		instance->Bind();
		instance->Draw();
	}
}