#pragma once
#include "../pch.h"
#include "../Rendering/Mesh.h"
#include "../Rendering/Material.h"


namespace PF
{
	template  <typename Key, typename AssetType>
	struct AssetMapBase : public std::map<std::string, Owns<AssetType>>
	{
		bool HasAsset(const std::string& path)
		{
			return find(path) != end();
		}
		AssetType* GetAsset(const std::string& path)
		{
			std::map<std::string, Owns<AssetType>>::iterator it = find(path);
			if (it != end())
			{
				return operator[path].get();
			}

			return nullptr;
		}
	};
		
	template  <typename AssetType>
	struct AssetMap : public AssetMapBase<std::string, AssetType>
	{

		



	};

	struct AssetsManager
	{
	protected:
		UInt lastId = 0;
		AssetMap<Texture> textures;
		AssetMap<Model> models;
		AssetMap<Material> materials;
		AssetMap<ShaderProgram> shaders;

	public:
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

		bool LoadModel(const std::string& key, const std::string& path) {

			Owns<Model> model(new Model());
			auto& gpum = model->meshes;
			auto& loader = model->meshData;

			if (!loader.LoadFile(path))
			{
				//PF_ERROR("Failed to load model {0}", objPath);
				__debugbreak();
			}


			auto gpumeshes = loader.GPUInstantiate();

			gpum.reserve(gpumeshes.size());
			std::transform(gpumeshes.begin(), gpumeshes.end(), std::back_inserter(gpum), [](GPUMesh* d)-> Owns<GPUMesh> {
				return Owns<GPUMesh>(d);
				});

			model->name = key;
			model->path = path;
			models[key] = std::move(model);

			for (auto& mat : loader.LoadedMaterials)
			{
				
					Owns<Material> mPtr{ new Material() };
					ShaderProgram* s;

					*mPtr = mat;
					
				/*	if ((s = shaders.GetAsset(std::to_string(mat.illum))))
					{*/
						s = new ShaderProgram({
						"../assets/shaders/COOK.vert",
						"../assets/shaders/COOK.frag"
						});
						shaders[std::to_string(mat.illum)].reset(s);
					//}

					mPtr->shader.reset(s);
					mPtr->name = mat.name;

					//textures[] = Texture::TextureFromFile(mat.map_d);
					materials[mat.name] = std::move(mPtr);
				
			}


			for (int ii = 0; ii < gpumeshes.size(); ii++)
			{
				gpumeshes[ii]->defaultMaterial = materials[loader.LoadedMeshes[ii].MeshMaterial.name].get();
			}
			//auto* m = loader.GPUInstantiate();
			//auto it = assets.emplace(path, m);

			return true; // *it.first->second;
		}
	};
}
