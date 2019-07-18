#pragma once




#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>

//#define DEBUG_OBJ_LOADER




#pragma region Models
//#define DEBUG_OBJ_LOADER
#ifdef DEBUG_OBJ_LOADER
#define DEBUG_PRINT(x) 
#else
#define DEBUG_PRINT(x) 
#endif // DEBUG

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

enum class MapType {
	AMBIENT,
	DIFFUSE,
	SPECULAR,
	SHINY,
	DISPLACEMENT,
	DECAL,
	BUMP,
	REFLECTION,
	DISSOLVE
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
public:

	Texture() {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, static_cast<int>(clamp), static_cast<int>(texInterpolation));
		glTexParameteri(GL_TEXTURE_2D, static_cast<int>(clamp), static_cast<int>(texInterpolation));
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
	MapType type = MapType::AMBIENT;
	float borderColor[4] = { 1,1,1,1 };
	Texture(const std::string path, MapType t) 
		: type(t)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, static_cast<int>(clamp), static_cast<int>(texInterpolation));
		glTexParameteri(GL_TEXTURE_2D, static_cast<int>(clamp), static_cast<int>(texInterpolation));

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
	}
};

class CubeMap {
public:
	CubeMap(const std::vector<std::string> paths) {

	}
	//std::array<Texture, 6> texs;
	//float cubeVertices[] = {
	//	// positions          // texture Coords
	//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	//	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	//	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	//};
	//float skyboxVertices[] = {
	//	// positions          
	//	-1.0f,  1.0f, -1.0f,
	//	-1.0f, -1.0f, -1.0f,
	//	1.0f, -1.0f, -1.0f,
	//	1.0f, -1.0f, -1.0f,
	//	1.0f,  1.0f, -1.0f,
	//	-1.0f,  1.0f, -1.0f,

	//	-1.0f, -1.0f,  1.0f,
	//	-1.0f, -1.0f, -1.0f,
	//	-1.0f,  1.0f, -1.0f,
	//	-1.0f,  1.0f, -1.0f,
	//	-1.0f,  1.0f,  1.0f,
	//	-1.0f, -1.0f,  1.0f,

	//	1.0f, -1.0f, -1.0f,
	//	1.0f, -1.0f,  1.0f,
	//	1.0f,  1.0f,  1.0f,
	//	1.0f,  1.0f,  1.0f,
	//	1.0f,  1.0f, -1.0f,
	//	1.0f, -1.0f, -1.0f,

	//	-1.0f, -1.0f,  1.0f,
	//	-1.0f,  1.0f,  1.0f,
	//	1.0f,  1.0f,  1.0f,
	//	1.0f,  1.0f,  1.0f,
	//	1.0f, -1.0f,  1.0f,
	//	-1.0f, -1.0f,  1.0f,

	//	-1.0f,  1.0f, -1.0f,
	//	1.0f,  1.0f, -1.0f,
	//	1.0f,  1.0f,  1.0f,
	//	1.0f,  1.0f,  1.0f,
	//	-1.0f,  1.0f,  1.0f,
	//	-1.0f,  1.0f, -1.0f,

	//	-1.0f, -1.0f, -1.0f,
	//	-1.0f, -1.0f,  1.0f,
	//	1.0f, -1.0f, -1.0f,
	//	1.0f, -1.0f, -1.0f,
	//	-1.0f, -1.0f,  1.0f,
	//	1.0f, -1.0f,  1.0f
	//};
	//


	//	unsigned int cubeVAO, cubeVBO;
	//	unsigned int skyboxVAO, skyboxVBO;
	//
	//void Render() {

	//	// cube VAO
	//	glGenVertexArrays(1, &cubeVAO);
	//	glGenBuffers(1, &cubeVBO);
	//	glBindVertexArray(cubeVAO);
	//	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	//	glEnableVertexAttribArray(0);
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//	glEnableVertexAttribArray(1);
	//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//	// skybox VAO
	//	glGenVertexArrays(1, &skyboxVAO);
	//	glGenBuffers(1, &skyboxVBO);
	//	glBindVertexArray(skyboxVAO);
	//	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	//	glEnableVertexAttribArray(0);
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);



	//}

};

#pragma endregion Texture

class SubMesh : public Asset{
public:
	Mesh & group;
	SubMesh(Mesh& g, std::string n)
		: group(g), Asset(n)
	{
	}
};

std::istream& operator>> (std::istream& file, SubMesh& mesh) {
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

		std::cout << buffer << std::endl;
		if (tofData[0] == 'v')
		{

			if (tofData.size() > 1) {
				switch (tofData[1])
				{
				case 'n':
				{
					stream >> vn;
					//mesh->vertexNormal.push_back(vn);
					//vertexData.push_back(vn);
					DEBUG_PRINT(std::cout << "(" << vn[0] << ", " << vn[1] << ", " << vn[2] << ")" << std::endl);

				}

					break;

				case 't':
					// while(!stream.eof()) // por ahora no triangula
				{
					stream >> vt[0] >> vt[1];
					v[2] = 0;
					//mesh->vertex.push_back(vt);
					//vertexData.push_back(Vec3(vt[0], vt[1], 0.f));
					DEBUG_PRINT(std::cout << "(" << vt[0] << ", " << vt[1] << ")" << std::endl);
				}


					break;

				default:
					break;
				}
			}
			else
			{
				// Position
				// while(!stream.eof()) //por ahora no triangula
				{
					DEBUG_PRINT(std::cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")" << std::endl);
				}
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

				stream >> f;

				//mesh->face.push_back(f);
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

				//mesh->face.push_back(f);
				//mesh->faceTex.push_back(ft);
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

					//mesh->face.push_back(f);
					//mesh->faceNorm.push_back(fn);

				}
				else
				{ //caras texturas y normales
					for (int ii = 0; ii < 3; ii++)
					{
						stream >> buffer;
						sscanf_s(buffer.c_str(), "%d/%d/%d", &f[ii], &ft[ii], &fn[ii]);
						DEBUG_PRINT(std::cout << "(" << f[ii] << ", " << ft[ii] << ", " << fn[ii] << ")" << std::endl);
					}

					//mesh->face.push_back(f);
					//mesh->faceNorm.push_back(fn);
					//mesh->faceTex.push_back(ft);
				}
			}

			break;
			}
		}
		else if (tofData == "usemtl")
		{
			stream >> buffer;// pongo el nombre del material
			//mesh->materials.push_back(materials[buffer]);
		}
	}
	return file;
}

class Material;
class ShaderProgram;
std::istream& operator>> (std::istream& file, Material& material);

class Material : public Asset {
public:
	ShaderProgram *shader = nullptr;
	
	
	void Bind() {
		//shader.setFloat("shininess", shiny);
		//////shader.setFloat("roughness", mat.rough);
		////shader.setFloat("covariance", mat.covariance);
		////shader.setFloat("reflectance", mat.reflectance);
		//shader.setVec3("kD", kD);
		//shader.setVec3("kS", kS);
		//shader.setVec3("kE", kE);
		//shader.setVec3("kA", kA);
	}


	std::map<std::string, Texture*> maps;
	Material(std::string n) : Asset(n) {

	}
	Vec3 kA;
	Vec3 kD;
	Vec3 kS;
	Vec3 kE{ 0,0,0 };
	float shiny = 0;
	float refractionIndex = 1;
	float dissolve = 1;

	//http://paulbourke.net/dataformats/mtl/
	static std::vector<Material*> ReadMTLLIB(std::string matPath) {

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
			if (tofData == "newmtl") {
				std::cout << buffer << std::endl;
				stream >> tofData;
				Material *mat(new Material(tofData));
				file >> *mat;
				materials.push_back(mat);
			}
		}
		DEBUG_PRINT(std::cout << "--------------------------" << std::endl);
		DEBUG_PRINT(std::cout << "--------------------------" << std::endl);

		return materials;
	}

};

std::istream& operator>> (std::istream& file, Material& material)
{
	std::string buffer;
	while (!file.eof() && file.good())
	{
		std::string tofData;

		if (file.peek() == 'n') {
			return file;
		}

		std::getline(file, buffer);

		std::stringstream stream(buffer);

		stream >> tofData;
		std::cout << buffer << std::endl;
		{


			// para los K falta espectral y xyz
			if (tofData == "Ka") {
				stream >> material.kA;
			}
			else if (tofData == "Kd") {
				stream >> material.kD;
			}
			else if (tofData == "Ks") {
				stream >> material.kS;
			}
			else if (tofData == "Ns") {
				stream >> material.shiny;
			}
			else if (tofData == "Ni") {
				stream >> material.refractionIndex;
			}
			else if (tofData == "d") {
				stream >> material.dissolve;
			}
			else if (tofData == "Tf") {

			}
			else if (tofData == "illum") {
				int type;
				stream >> type;
				material.shader = ShaderProgram::GetDefault(static_cast<IllumModel>(type));
			}
			else if (tofData == "map_Ka") {

				stream >> tofData;

				if (material.maps.find(tofData) == material.maps.end())
				{
					material.maps[tofData] = new Texture(tofData, MapType::AMBIENT);
				}

			}
			else if (tofData == "map_Kd") {

				stream >> tofData;

				if (material.maps.find(tofData) == material.maps.end())
				{
					material.maps[tofData] = new Texture(tofData, MapType::DIFFUSE);

				}

			}
			else if (tofData == "map_Ks") {


			}
			else if (tofData == "map_Ns") {
				stream >> tofData;

				if (material.maps.find(tofData) == material.maps.end())
				{
					material.maps[tofData] = new Texture(tofData, MapType::SHINY);

				}


			}
			else if (tofData == "map_d") {

			}
			else if (tofData == "disp") {
				stream >> tofData;

				if (material.maps.find(tofData) == material.maps.end())
				{
					material.maps[tofData] = new Texture(tofData, MapType::DISPLACEMENT);

				}

			}
			else if (tofData == "decal") {


			}
			else if (tofData == "bump") {

				stream >> tofData;

				if (material.maps.find(tofData) == material.maps.end())
				{
					material.maps[tofData] = new Texture(tofData, MapType::BUMP);

				}
			}
		}
	}

	return file;
}

//http://paulbourke.net/dataformats/obj/
class Mesh : public Asset {
public:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	std::vector<Vertex> vertex;
	std::vector<Material*> materials;
	std::vector<SubMesh*> submesh;

	SubMesh& AddSubMesh(std::string n) {
		SubMesh * m = new SubMesh(*this, n);
		submesh.push_back(m);
		return *m;
	}

	Mesh(const std::string path)
		: Asset(path)
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
					SubMesh &m = AddSubMesh(tofData);
					file >> m;
				}
				else if (tofData == "mtllib") {
					std::cout << buffer << std::endl;

					stream >> tofData;
					std::string matPath = path.substr(0, path.find_last_of("/"));
					matPath.append("/" + tofData);

					materials = Material::ReadMTLLIB(matPath);

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

		////for (auto mesh : meshes)
		////{

		////	mesh->face.pop_back();
		////}

		////for (auto mesh : meshes)
		////{

		////	size_t numVertex = mesh->face.size();

		////	std::vector<std::vector<iVec3>*> arrs({ &mesh->face, &mesh->faceNorm, &mesh->faceTex });
		////	std::vector<std::vector<Vec3>*> arrsTarg({ &mesh->vertex, &mesh->vertexNormal, &mesh->uv });
		////	std::vector<Vec3> aux;

		////	for (unsigned int ii = 0; ii < arrs.size(); ii++) {
		////		auto &arr = *arrs[ii];
		////		auto &arrTarget = *arrsTarg[ii];

		////		for (auto &f : arr) {
		////			//std::cout << f.x << " " << f.y << " " << f.z << std::endl;

		////			for (int ii = 0; ii < 3; ii++) {
		////				glm::u32& index = f[ii];
		////				if (index > 0) {
		////					index--;
		////				}
		////				else {
		////					index = (unsigned int)(numVertex + index);
		////				}


		////			}
		////			aux.push_back(arrTarget[f[0]]);
		////			aux.push_back(arrTarget[f[1]]);
		////			aux.push_back(arrTarget[f[2]]);

		////		}
		////		arrTarget = std::move(aux);


		////	}


		////}


	}

	void GLCreate() {

		glBindVertexArray(VAO);
		// 2. copy our vertices array in a vertex buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertex.size(), vertex.data() , GL_STATIC_DRAW);
		// 3. copy our index array in a element buffer for OpenGL to use
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// 4. then set the vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

	}

};
#pragma endregion Models



