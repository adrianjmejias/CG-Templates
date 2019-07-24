#pragma once
#include "types.h"
#include "OBJ_Loader.h"


void Assign(Vec3& a, const objl::Vector3& b);
void Assign(Vec2& a, const objl::Vector2& b);

struct Vertex {
	Vec3 pos;
	Vec3 normal;
	Vec2 uv;
	Vec3 bitan;
	Vec3 tan;
};



class Material 
	: public objl::Material 
{
	unsigned int smap_kD = 0;
};

class Model
	: public objl::Loader
{
public:
	Model();
	~Model();
};

class Mesh  
	//public objl::Mesh
{
public:
	std::vector<Vertex> vertex;
	std::vector<glm::uvec3> indices;
	Material mat;
	unsigned int VAO = 0;
	unsigned int nIndices = 0;
	unsigned int EBO = 0;
	unsigned int VBO = 0;
};

