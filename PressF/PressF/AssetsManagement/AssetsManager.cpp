#include "PressF/pch.h"

#include "AssetsManager.h"
#include "PressF/Rendering/ModelImporter.h"
#include "PressF/Rendering/loglMesh.h"
//#include "PressF/Rendering/MaterialSystem/Material.h"
namespace PF
{

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

	Ref<Model> AssetsManager::LoadModel(const std::string& key, const std::string& path) {

		//ModelImporter importer(path, true);

		Model* m;
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_ForceGenNormals | aiProcess_CalcTangentSpace);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			PF_WARN("ERROR::ASSIMP:: {}", importer.GetErrorString());
			return false;
		}
		// retrieve the directory path of the filepath
		const std::string directory = path.substr(0, path.find_last_of('/'));
		m = new Model();
		actModel = m;
		auto& meshes = m->meshes;
		std::queue<aiNode*> nodes;

		nodes.push(scene->mRootNode);
		Ref<ShaderProgram> s;


		
		s = LoadShader("defaultShader", "../assets/shaders/COOK.vert", "../assets/shaders/COOK.frag");
		while (!nodes.empty())
		{
			aiNode* act = { nodes.front() };
			nodes.pop();

			for (unsigned int i = 0; i < act->mNumMeshes; i++)
			{
				// the node object only contains indices to index the actual objects in the scene. 
				// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
				aiMesh* mesh = scene->mMeshes[act->mMeshes[i]];

				
				meshes.emplace_back (ProcessMesh(mesh, scene, directory));


			}


			for (unsigned int ii = 0; ii < act->mNumChildren; ii++)
			{
				nodes.push(act->mChildren[ii]);
			}
		}

		models[key].reset(m);

		return m; 
	}

	GPUMesh* AssetsManager::ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory)
	{
		// data to fill
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<PF::Texture> textures;
		GPUMesh* mptr{ new GPUMesh() };
		GPUMesh& m{ *mptr };
		m.name = mesh->mName.C_Str();
		// Walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
							  // positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			if (mesh->HasTangentsAndBitangents())
			{
				//mesh->
				// tangent
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent = vector;

				// bitangent
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.Bitangent = vector;
			}
			vertices.push_back(vertex);
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// process materials
		/*   aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
		Same applies to other texture as the following list summarizes:
		diffuse: texture_diffuseN
		specular: texture_specularN
		normal: texture_normalN*/


		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		auto itm = materials.find(material->GetName().C_Str());
		//material->Get()
		Material* mat{ nullptr };
		if (itm == materials.end())
		{
			mat =  new Material();
			actModel->materials.push_back(mat);
			mat->shader = shaders["defaultShader"].get();
	/*		
			material->Get("COLOR_DIFFUSE", kd);
			*/
			Vec3 kd;
		
			aiColor3D color(0.f, 0.f, 0.f);
			material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			kd.r = color.r;
			kd.g = color.g;
			kd.b = color.b;
			mat->AddParameter<ShaderVec3>("kD", kd);
			// 1. diffuse maps
			loadMaterialTextures(material, mat, directory, aiTextureType_DIFFUSE, TexMaterialRole::Diffuse);
			// 2. specular maps
			loadMaterialTextures(material,  mat, directory, aiTextureType_SPECULAR, "texture_specular");
			//// 3. normal maps
			loadMaterialTextures(material, mat, directory, aiTextureType_HEIGHT, "texture_normal");
			//// 4. height maps
			loadMaterialTextures(material, mat, directory, aiTextureType_AMBIENT, "texture_height");

			// return a mesh object created from the extracted mesh data

			materials[material->GetName().C_Str()].reset(mat);
		}
		else
		{
			mat =  itm->second.get();
		}
		m.SetupMesh(vertices, indices, textures);
		m.mat = mat;
	
		//mat->AddParameter();
		return mptr;
	}

	void AssetsManager::loadMaterialTextures(aiMaterial* mat, Material *mmat, const std::string& directory, aiTextureType type, const std::string& role)
	{
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			auto it = textures.find(str.C_Str());
			if (it == textures.end())
			{
				std::string path = directory +"/"+ str.C_Str();
				Owns<Texture> texPtr{ Texture::TextureFromFile(path) };

				if (texPtr.get())
				{
					mmat->AddTexture(role, *texPtr);
					textures[str.C_Str()] = std::move(texPtr);
				}
				else
				{
					PF_WARN("Texture not found: {}", path);
				}
			}

		}
	}

}