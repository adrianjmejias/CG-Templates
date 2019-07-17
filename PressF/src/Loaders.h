#pragma once




#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>


//#define DEBUG_OBJ_LOADER
#ifdef DEBUG_OBJ_LOADER
#define DEBUG_PRINT(x) (x)
#else
#define DEBUG_PRINT(x) 
#endif // DEBUG



#pragma region Models

using Face = iVec3;

class Vertex {
public:
	Vec3 pos;
	Vec3 uv;
	Vec3 normal;
	Vec3 tangent;
	Vec3 bitangent;
};

#pragma region Texture

enum class ClampType {
	REPEAT = GL_REPEAT,
	MIRROR_REPEAT = GL_MIRRORED_REPEAT,
	EDGE = GL_CLAMP_TO_EDGE,
	BORDER = GL_CLAMP_TO_BORDER,
};

enum class TexInterpolation {
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR,
};



class Texture {
	//int width, height, nrChannels;
	//unsigned char *data;
	//for (GLuint i = 0; i < textures_faces.size(); i++)
	//{
	//	data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
	//	glTexImage2D(
	//		GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
	//		0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
	//	);
	//}

	Texture(const std::string path) {
		//this->path = path;
		LoadTexture(path);
	}

	~Texture() {
		glDeleteTextures(1, &id);
	}

	unsigned int id;
	int width;
	int height;
	int nChannels;
	ClampType clamp = ClampType::REPEAT;
	TexInterpolation texInterpolation = TexInterpolation::LINEAR;

	float borderColor[4] = { 1,1,1,1 };
	unsigned int LoadTexture(const std::string path) {
		// Creates the texture on GPU
		glGenTextures(1, &id);
		// Loads the texture
		// Flips the texture when loads it because in opengl the texture coordinates are flipped
		stbi_set_flip_vertically_on_load(true);
		// Loads the texture file data
		unsigned char *data = stbi_load(path.data(), &width, &height, &nChannels, 0);
		if (data)
		{
			// Gets the texture channel format
			GLenum format;
			switch (nChannels)
			{
			case 1:
				format = GL_RED;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			}

			// Binds the texture
			glBindTexture(GL_TEXTURE_2D, id);
			// Creates the texture
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			// Creates the texture mipmaps
			glGenerateMipmap(GL_TEXTURE_2D);

			// Set the filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<int>(clamp));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<int>(clamp));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(texInterpolation));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(texInterpolation));

		}
		else
		{
			PF_ERROR("ERROR:: Unable to load texture ", path);
			glDeleteTextures(1, &id);
		}
		// We dont need the data texture anymore because is loaded on the GPU
		stbi_image_free(data);
		PF_INFO("texture loaded {0}", path);
		return id;
	}
};

class CubeMap {
	std::array<Texture, 6> texs;



};

#pragma endregion Texture




class Material : public Asset {
public:
	Vec3 kA;
	Vec3 kD;
	Vec3 kS;
	Vec3 kE;
};

class Mesh : public Asset {
public:
	std::vector<Vertex> vertex;
	std::vector<iVec3> face;
};


#pragma endregion Models



static std::vector<Material*> readMTLLIB(std::string folderPath, std::string nameMaterial);
static Material* readMaterial(std::ifstream &file, std::string nameMaterial);
static Mesh* readO(std::ifstream&, std::string, std::string, std::vector<Vec3>&, std::unordered_map<std::string, std::shared_ptr<Material>>&);


//http://paulbourke.net/dataformats/obj/
std::vector<Mesh*> loadObj(const std::string path)
{
	std::ifstream file(path, std::ios::in);
	std::string buffer;

	if (!file.is_open())
	{
		DEBUG_PRINT(std::cout << "Couldn't open file" << path << std::endl);
		__debugbreak();
		std::cin.ignore();
		exit(-1);
	}

	std::vector<Vec3> vertexData;
	std::vector<Mesh*> meshes;
	std::unordered_map<std::string, std::shared_ptr<Material>> materials;


	while (!file.eof() && file.good())
	{
		std::getline(file, buffer);
		std::stringstream stream(buffer);
		DEBUG_PRINT(std::cout << buffer << std::endl);
		{
			std::string tofData;
			stream >> tofData;

			if (tofData[0] == '#')
			{
				continue;
			}

			if (tofData == "o") {
				stream >> tofData;
				Mesh * m = readO(file, tofData, path, vertexData, materials);
				meshes.push_back(m);

				std::cout << m->name << std::endl;

			}
			else if (tofData == "mtllib") {
				std::cout << buffer << std::endl;

				stream >> tofData;
				std::string matPath = path.substr(0, path.find_last_of("/"));
				matPath.append("/" + tofData);

				std::vector<Material*> mats = readMTLLIB(matPath, buffer);
				for (auto mat : mats) {
					materials[mat->name].reset(mat);
				}
			}
			else if (tofData == "s")
			{
				stream >> buffer;

				// if
			}
		}
		DEBUG_PRINT(std::cout << "--------------------------" << std::endl);
		DEBUG_PRINT(std::cout << "--------------------------" << std::endl);
	}

	for (auto mesh : meshes)
	{

		mesh->face.pop_back();
	}

	for (auto mesh : meshes)
	{

		size_t numVertex = mesh->face.size();

		std::vector<std::vector<iVec3>*> arrs({ &mesh->face, &mesh->faceNorm, &mesh->faceTex });
		std::vector<std::vector<Vec3>*> arrsTarg({ &mesh->vertex, &mesh->vertexNormal, &mesh->uv });
		std::vector<Vec3> aux;

		for (unsigned int ii = 0; ii < arrs.size(); ii++) {
			auto &arr = *arrs[ii];
			auto &arrTarget = *arrsTarg[ii];

			for (auto &f : arr) {
				//std::cout << f.x << " " << f.y << " " << f.z << std::endl;

				for (int ii = 0; ii < 3; ii++) {
					glm::u32& index = f[ii];
					if (index > 0) {
						index--;
					}
					else {
						index = (unsigned int)(numVertex + index);
					}


				}
				aux.push_back(arrTarget[f[0]]);
				aux.push_back(arrTarget[f[1]]);
				aux.push_back(arrTarget[f[2]]);

			}
			arrTarget = std::move(aux);


		}


	}


	return meshes;
}



static Mesh* readO(std::ifstream &file, std::string name, std::string path, std::vector<Vec3> &vertexData, std::unordered_map<std::string, std::shared_ptr<Material>>& materials) {
	Mesh *mesh(new Mesh());
	mesh->path = path;
	mesh->name = name;
	Vec3 v;
	Vec3 vt;
	Vec3 vn;

	iVec3 f;
	iVec3 ft;
	iVec3 fn;
	std::string tofData;


	while (!file.eof() && file.good() && file.peek() != 'o')
	{
		std::string buffer;
		std::getline(file, buffer);
		std::stringstream stream(buffer);

		stream >> tofData;


		if (tofData == "v")
		{
			// while(!stream.eof()) //por ahora no triangula
			{
				stream >> v[0] >> v[1] >> v[2];

				mesh->vertex.push_back(v);
				vertexData.push_back(v);
				DEBUG_PRINT(std::cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")" << std::endl);
			}
		}
		else if (tofData == "vn")
		{
			mesh->hasNormals = true;

			// while(!stream.eof()) // por ahora no triangula
			{
				stream >> vn[0] >> vn[1] >> vn[2];
				mesh->vertexNormal.push_back(vn);
				vertexData.push_back(vn);
				DEBUG_PRINT(std::cout << "(" << vn[0] << ", " << vn[1] << ", " << vn[2] << ")" << std::endl);

			}
		}
		else if (tofData == "vt")
		{
			mesh->hasUVs = true;
			// while(!stream.eof()) // por ahora no triangula
			{
				stream >> vt[0] >> vt[1];

				mesh->uv.push_back(Vec3(vt[0], vt[1], 0.f));
				vertexData.push_back(Vec3(vt[0], vt[1], 0.f));
				DEBUG_PRINT(std::cout << "(" << vt[0] << ", " << vt[1] << ")" << std::endl);
			}
		}
		else if (tofData == "f")
		{
			auto posInit = stream.tellg();
			stream >> buffer;
			stream.seekg(posInit); // hago como si no hubiera leido nada del stream

								   //x case f v1 v2 v3 ....
								   //x case f v1/vt1 v2/vt2 v3/vt3 ...
								   // case f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
								   // case f v1//vn1 v2//vn2 v3//vn3 ...

			bool hasDoubleSlash = false;
			unsigned int countDeSlash = 0;
			{
				unsigned int slashPos = buffer.find_first_of("/");
				hasDoubleSlash = (buffer[slashPos + 1]) == '/';
			}
			countDeSlash = std::count(buffer.begin(), buffer.end(), '/');

			DEBUG_PRINT(std::cout << hasDoubleSlash << " " << countDeSlash << std::endl);

			//std::vector<std::string> tokens;




			switch (countDeSlash)
			{
			case 0: // solo caras
			{

				stream >> f[0] >> f[1] >> f[2];

				mesh->face.push_back(f);
				DEBUG_PRINT(std::cout << "(" << f[0] << ", " << f[1] << ", " << f[2] << ")" << std::endl);
			}
			break;
			case 1: // caras y texturas
			{
				for (int ii = 0; ii < 3; ii++)
				{
					stream >> buffer;
					sscanf_s(buffer.c_str(), "%d/%d", &f[ii], &ft[ii]);

					DEBUG_PRINT(std::cout << "(" << f[ii] << ", " << ft[ii] << ")" << std::endl);
				}

				mesh->face.push_back(f);
				mesh->faceTex.push_back(ft);
			}
			break;
			case 2:
			{
				if (hasDoubleSlash)
				{ // caras y normales

					for (int ii = 0; ii < 3; ii++)
					{
						stream >> buffer;
						sscanf_s(buffer.c_str(), "%d//%d", &f[ii], &fn[ii]);
						DEBUG_PRINT(std::cout << "(" << f[ii] << ", " << fn[ii] << ")" << std::endl);
					}

					mesh->face.push_back(f);
					mesh->faceNorm.push_back(fn);

				}
				else
				{ //caras texturas y normales
					for (int ii = 0; ii < 3; ii++)
					{
						stream >> buffer;
						sscanf_s(buffer.c_str(), "%d/%d/%d", &f[ii], &ft[ii], &fn[ii]);
						DEBUG_PRINT(std::cout << "(" << f[ii] << ", " << ft[ii] << ", " << fn[ii] << ")" << std::endl);
					}

					mesh->face.push_back(f);
					mesh->faceNorm.push_back(fn);
					mesh->faceTex.push_back(ft);
				}
			}

			break;
			}
		}
		else if (tofData == "usemtl")
		{
			stream >> buffer;// pongo el nombre del material
			mesh->materials.push_back(materials[buffer]);
		}
	}

	return mesh;
}



//http://paulbourke.net/dataformats/mtl/
std::vector<Material*> readMTLLIB(std::string matPath, std::string nameMaterial) {

	std::vector<Material*> materials;
	std::ifstream file(matPath, std::ios::in);

	if (!file.is_open())
	{
		DEBUG_PRINT(std::cout << "Couldn't open material file" << matPath << std::endl);
		__debugbreak();

		return materials;
	}

	std::string buffer;
	while (!file.eof() && file.good())
	{
		std::string tofData;
		std::getline(file, buffer);

		std::stringstream stream(buffer);

		stream >> tofData;
		DEBUG_PRINT(std::cout << buffer << std::endl);
		{
			if (tofData == "newmtl") {
				std::cout << buffer << std::endl;
				stream >> tofData;

				materials.push_back(readMaterial(file, tofData));
			}
		}
	}
	DEBUG_PRINT(std::cout << "--------------------------" << std::endl);
	DEBUG_PRINT(std::cout << "--------------------------" << std::endl);

	return materials;
}


static Material* readMaterial(std::ifstream &file, std::string nameMaterial) {
	Material *material(new Material());
	material->name = nameMaterial;

	std::string buffer;
	while (!file.eof() && file.good())
	{
		std::string tofData;

		if (file.peek() == 'n') {
			return material;
		}

		std::getline(file, buffer);

		std::stringstream stream(buffer);

		stream >> tofData;
		DEBUG_PRINT(std::cout << buffer << std::endl);
		{


			// para los K falta espectral y xyz
			if (tofData == "Ka") {
				stream >> material->color[0];
				stream >> material->color[1];
				stream >> material->color[2];


			}
			else if (tofData == "Kd") {
				stream >> material->kD
			}
			else if (tofData == "Ks") {
				stream >> material->specular[0];
				stream >> material->specular[1];
				stream >> material->specular[2];
			}
			else if (tofData == "Ns") {
				stream >> material->shiny;
			}
			else if (tofData == "Ni") {
				stream >> material->refractionIndex;
			}
			else if (tofData == "d") {
				stream >> material->dissolve;
			}
			else if (tofData == "bump") {

			}
			else if (tofData == "Tf") {

			}
			else if (tofData == "illum") {
				int type;
				stream >> type;

				//switch (type)
				//{
				//	//del 0 al 10 creo XD
				//default:
				//	break;
				//}
			}
		}
	}

	return material;
}




//void readRFL(); // esto es para el espectral de los K