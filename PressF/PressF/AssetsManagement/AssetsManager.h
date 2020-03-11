#pragma once
#include "../pch.h"
#include <map>
#include <string>
#include <memory>

#include "../Rendering/Mesh.h"
#include "../Rendering/Material.h"


namespace PF
{
	template  <typename AssetType>
	struct AssetMap : std::map<std::string, Owns<AssetType>>
	{

		bool HasAsset(std::string path)
		{
			return find(path) != end();
		}
	};

	class AssetsManager
	{
	protected:
		UInt lastId = 0;
		AssetMap<Texture> textures;
		AssetMap<Model> models;
		AssetMap<Material> materials;
	public:


		virtual bool LoadModel(const std::string& path) {
			CPUMesh loader;

			if (!loader.LoadFile(path))
			{
				//PF_ERROR("Failed to load model {0}", objPath);
				__debugbreak();
			}


			for (auto& mat : loader.LoadedMaterials)
			{
				if (!materials.HasAsset(mat.name))
				{
					Owns<Material> mPtr{ new Material() };

					
					mPtr->name = std::move(mat.name);
					*mPtr = mat;
					
					//textures[] = Texture::TextureFromFile(mat.map_d);


					materials[mat.name] = std::move(mPtr);
				}
			}
			auto* m = loader.GPUInstantiate();
			//auto it = assets.emplace(path, m);

			return true; // *it.first->second;
		}




	private:
	};
}
