#pragma once

#include "pch.h"
//#include "Physics.h"
#
//#include "Texture.h"
#include "Rendering/Mesh.h"

namespace PF
{

//class MeshRenderer;







//inline Model * Application::GLCreate(objl::Loader & fullModel) {
//	Model * m = new Model();
//	Model &model = *m;
//	//data transform
//	//std::vector<Vertex> vertex;
//	//vertex.reserve(fullModel.LoadedVertices.size());
//	//for (const objl::Vertex& oVertex : fullModel.LoadedVertices) {
//	//	vertex.push_back(Vertex(oVertex));
//	//}
//
//	GLsizei totalIndices = 0;
//	model.reserve(fullModel.LoadedMeshes.size());
//
//
//
//
//
//	std::vector<Vertex> vertex{begin(fullModel.LoadedVertices), end(fullModel.LoadedVertices)};
//
//	std::vector<Vec3> tanPerFace;
//	tanPerFace.reserve(fullModel.LoadedIndices.size() / 3);
//
//	std::vector<std::vector<size_t> > vertexContrib{ vertex.size(), std::vector<size_t>() };
//
//	for (size_t iiFace = 0, iiReal = 0; iiReal < fullModel.LoadedIndices.size() - 2; iiFace++, iiReal += 3)
//	{
//		const int idx_a = fullModel.LoadedIndices[iiReal];
//		const int idx_b = fullModel.LoadedIndices[iiReal + 1];
//		const int idx_c = fullModel.LoadedIndices[iiReal + 2];
//
//		const Vertex &v_1 = vertex[idx_a];
//		const Vertex &v_2 = vertex[idx_b];
//		const Vertex &v_3 = vertex[idx_c];
//
//
//		// hago esto por legibilidad
//		const Vec3 &u{ v_1.uv[0],v_2.uv[0],v_3.uv[0] };
//		const Vec3 &v{ v_1.uv[1],v_2.uv[1],v_3.uv[1] };
//		const Vec3 p[3]{ v_1.pos,v_2.pos ,v_3.pos };
//
//
//		const float v20 = v[2] - v[0];
//		const float v10 = v[1] - v[0];
//
//		const float u10 = u[1] - u[0];
//		const float u20 = u[2] - u[0];
//
//		const Vec3 p10 = p[1] - p[0];
//		const Vec3 p20 = p[2] - p[0];
//
//
//		const Vec3 tan(((v20*p10) - (v10 * p20)) / ((u10*v20) - (v10*u20)));
//
//
//		tanPerFace.push_back(tan);
//
//		vertexContrib[idx_a].push_back(iiFace);
//		vertexContrib[idx_b].push_back(iiFace);
//		vertexContrib[idx_c].push_back(iiFace);
//	}
//
//	for (size_t ii = 0; ii < vertexContrib.size(); ii++)
//	{
//		Vec3 promTan{ 0,0,0 };
//
//
//		for (size_t indexFace : vertexContrib[ii])
//		{
//			promTan += tanPerFace[indexFace];
//		}
//
//		vertex[ii].tan = glm::normalize(promTan / static_cast<float>(vertexContrib[ii].size()));
//	}
//
//	for (int ii = 0; ii < fullModel.LoadedMeshes.size(); ii++) {
//		objl::Mesh& objlMesh = fullModel.LoadedMeshes[ii];
//		Mesh myMesh;
//		myMesh.name = objlMesh.MeshName;
//
//		std::vector<Vertex> meshVertex{begin(objlMesh.Vertices), end(objlMesh.Vertices) };
//		
//
//		myMesh.mat = objlMesh.MeshMaterial;
//		myMesh.nElem = static_cast<GLsizei>(objlMesh.Indices.size());
//		myMesh.offset = totalIndices;
//
//		totalIndices += myMesh.nElem;
//
//		GLCALL(glGenVertexArrays(1, &myMesh.VAO));
//		GLCALL(glGenBuffers(1, &myMesh.EBO));
//		GLCALL(glGenBuffers(1, &myMesh.VBO));
//
//		GLCALL(glBindVertexArray(myMesh.VAO));
//		{
//			GLCALL(glBindBuffer(GL_ARRAY_BUFFER, myMesh.VBO));
//			GLCALL(glBufferData(GL_ARRAY_BUFFER, meshVertex.size() * sizeof(Vertex), meshVertex.data(), GL_STATIC_DRAW));
//
//			GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myMesh.EBO));
//			GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, objlMesh.Indices.size() * sizeof(unsigned int), objlMesh.Indices.data(), GL_STATIC_DRAW));
//
//			{
//				GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos)));
//				GLCALL(glEnableVertexAttribArray(0));
//
//				GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));
//				GLCALL(glEnableVertexAttribArray(1));
//
//				GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv)));
//				GLCALL(glEnableVertexAttribArray(2));
//
//				GLCALL(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tan)));
//				GLCALL(glEnableVertexAttribArray(3));
//
//			}
//
//		}
//		GLCALL(glBindVertexArray(0));
//		model.push_back(myMesh);
//
//	}
//	
//	return m;
//}



}
