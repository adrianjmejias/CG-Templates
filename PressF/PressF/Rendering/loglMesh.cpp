#pragma once
#include "PressF/pch.h"
#include "loglMesh.h"

namespace PF
{
	void GPUMesh::Bind()
	{
		VAO.Bind();
        //GLCALL(glBindVertexArray(VAO));

	}

	void GPUMesh::Render()
	{

		GLCALL(glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0));
	}

    void GPUMesh::SetupMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<PF::Texture>& textures)
    {  
        
        // create buffers/arrays
        glGenVertexArrays(1, &VAO.id);
        glGenBuffers(1, &VBO.id);
        glGenBuffers(1, &EBO.id);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

        glBindVertexArray(0);

        nIndices = (unsigned int)indices.size();
    }


	void RenderMask::SetMode(MeshRenderMode renderMode)
	{
		RenderMask& renderMask = *this;

		switch (renderMode)
		{
		case PF::MeshRenderMode::Shiny:
			renderMask[PF_BLOOM] = true;

		case PF::MeshRenderMode::Normal:
			renderMask[PF_NORMAL] = true;
			renderMask[PF_TRANSPARENT] = false;

			break;
		case PF::MeshRenderMode::Transparent:
			renderMask[PF_TRANSPARENT] = true;
			renderMask[PF_NORMAL] = false;
			renderMask[PF_BLOOM] = false;
			break;
		default:
			break;
		}
	}
}