#pragma once
#include "PressF/pch.h"
#include "PressF/Rendering/Mesh.h"
#include "AssetMap.h"
//#include "PressF/Rendering/MaterialSystem/Material.h"

namespace PF
{

	struct AssetsManager
	{
	protected:
	public:
		UInt lastId{ 0 };
		AssetMap<Texture> textures{ "Texture Manager" };
		AssetMap<Model> models{ "Model Manager" };
		AssetMap<Material> materials{ "Material Manager" };
		AssetMap<ShaderProgram> shaders{ "ShaderProgram Manager" };


		virtual void ImGui();
		//Model* AddModel()
		//{

		//}
		//
		//Material* AddMaterial(Material* mat)
		//{
		//	//materials[mat->name].reset(mat);
		//}

		Model* GetModel(const std::string& key);

		bool LoadModel(const std::string& key, const std::string& path);
	};
}
