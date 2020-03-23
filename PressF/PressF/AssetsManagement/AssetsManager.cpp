#include "PressF/pch.h"

#include "AssetsManager.h"

namespace PF
{
	std::vector<Texture> ExtractTextures(objl::Material mat)
	{
		std::vector<Texture> texs;

		return texs;
	}



	void AssetsManager::ImGui()
	{
		textures.ImGui();
		models.ImGui();
		shaders.ImGui();
		materials.ImGui();
	}

	Model* AssetsManager::GetModel(const std::string& key)
	{
		return models[key].get();
	}

	bool AssetsManager::LoadModel(const std::string& key, const std::string& path) {

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

			mPtr->shader = (s);
			mPtr->name = mat.name;

			//textures[] = Texture::TextureFromFile(mat.map_d);
			materials[mat.name] = std::move(mPtr);

		}


		for (int ii = 0; ii < gpumeshes.size(); ii++)
		{
			gpumeshes[ii]->defaultMaterial = materials[loader.LoadedMeshes[ii].MeshMaterial.name].get();
		}

		return true; 
	}

}