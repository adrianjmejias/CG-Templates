#pragma once
#include "PressF/pch.h"
#include "PressF/Rendering/Mesh.h"
#include "PressF/Rendering/Material.h"
#include "AssetMap.h"

namespace PF
{

	struct AssetsManager
	{
	protected:
	public:
		UInt lastId{ 0 };
		AssetMap<Texture> textures;
		AssetMap<Model> models;
		AssetMap<Material> materials;
		AssetMap<ShaderProgram> shaders;

		Model* AddModel()
		{

		}
		
		Material* AddMaterial(Material* mat)
		{
			//materials[mat->name].reset(mat);
		}

		Model* GetModel(const std::string& key)
		{
			return models[key].get();
		}

		bool LoadModel(const std::string& key, const std::string& path);
	};
}
