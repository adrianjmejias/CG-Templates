#pragma once
#include "PressF/pch.h"
//#include <assimp/scene.h>
#include "PressF/AssetsManagement/Asset.h"
#include "PressF/Rendering/MaterialSystem/Material.h"


namespace PF
{
	constexpr void Assign(Vec3& a, const objl::Vector3& b);
	constexpr void Assign(Vec2& a, const objl::Vector2& b);
	class MeshRenderer;

	struct Vertex {
		Vec3 pos;
		Vec3 normal;
		Vec2 uv;
		Vec3 tan;
		Vertex(const objl::Vertex& v);
	};

	struct GPUMesh
	{
		std::string name;
		VertexArrayObject VAO;
		VertexBufferObject EBO, VBO;
		GLsizei offset =0, nElem=0;
		Ref<Material> defaultMaterial = nullptr;

		void Bind();

		void Render();
	};

	struct CPUMesh : public objl::Loader
	{
		std::string name;
		std::vector<GPUMesh*> GPUInstantiate();
	};

	struct Model : public Asset
	{
		CPUMesh meshData;
		std::vector<Owns<GPUMesh>> meshes;
		Model();
		~Model();
		virtual void ImGui();
	};
}