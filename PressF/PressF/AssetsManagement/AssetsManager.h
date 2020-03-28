#pragma once
#include "PressF/pch.h"
//#include "PressF/Rendering/Mesh.h"
#include "AssetMap.h"
#include "PressF/Rendering/loglModel.h"
#include "PressF/Rendering/loglMesh.h"
//#include "PressF/Rendering/MaterialSystem/Material.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>


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
		Ref<Model> actModel;

		virtual void ImGui();
		Model* GetModel(const std::string& key);


		
		Ref<Texture> LoadTexture(const std::string key, const std::string path)
		{
			auto it = textures.find(key);
			if (it == textures.end())
			{
				Owns<Texture> texPtr{ Texture::TextureFromFile(path) };
				if (texPtr.get())
				{
					Texture* t{ texPtr.get() };
					textures[key] = std::move(texPtr);
					return textures[key].get();
				}
				else
				{
					PF_WARN("Texture not found: {}", path);
				}
			}
			else
			{
				return it->second.get();
			}
			return nullptr;
		}
		Ref<ShaderProgram> LoadShader(const std::string key, const std::string vertPath, const std::string fragPath)
		{
			auto it = shaders.find(key);
			if (it == shaders.end())
			{
				Owns<ShaderProgram> sPtr{ ShaderProgram::FromFiles(vertPath, fragPath) };
				ShaderProgram* s{ sPtr.get() };
				shaders[key] = std::move(sPtr);

				return s;
			}
			else
			{
				return it->second.get();
			}

			return nullptr;
		}
		Ref<ShaderProgram> GetShader(const std::string& key)
		{
			auto it = shaders.find(key);

			if (it != shaders.end())
			{
				return it->second.get();
			}

			return nullptr;
		}
		Ref<Texture> GetTexture(const std::string& key)
		{
			auto it = textures.find(key);

			if (it != textures.end())
			{
				return it->second.get();
			}

			return nullptr;
		}

		
		Ref<Model> LoadModel(const std::string& key, const std::string& path);
		GPUMesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory);
		void loadMaterialTextures(aiMaterial* mat, Material* mmat, const std::string& directory, aiTextureType type, const std::string&);
	};
}
