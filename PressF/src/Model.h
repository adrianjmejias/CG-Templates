#pragma once
#include "types.h"
#include "OBJ_Loader.h"

class MeshRenderer;
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
{
public:
	std::string name;
	Vec3 kA;
	Vec3 kD;
	Vec3 kS;
	float Ns;
	float Ni;
	float d;
	int illum;
	std::string map_Ka;
	std::string map_Kd;
	std::string map_Ks;
	std::string map_Ns;
	std::string map_d;
	std::string map_bump;
	unsigned int smap_kD = 0;
	Material();
	Material(const objl::Material& m);
	Material& operator = (const objl::Material &m)
	{
		Assign(kA, m.Ka);
		Assign(kD, m.Kd);
		Assign(kS, m.Ks);
		Ns = m.Ns;
		Ni = m.Ni;
		d = m.d;
		illum = m.illum;
		map_Ka = m.map_Ka;
		map_Kd = m.map_Kd;
		map_Ks = m.map_Ks;
		map_Ns = m.map_Ns;
		map_d = m.map_d;
		map_bump = m.map_bump;
		return *this;
	}
};


class Mesh  
	: public std::vector<MeshRenderer*>
{
public:
	unsigned int offset, nElem;
	std::vector<Vertex> vertex;
	std::vector<glm::uvec3> indices;
	Material mat;
};

class Model
	: public std::vector<Mesh>
{
public:
	unsigned int VAO, EBO, VBO;

	Model();
	~Model();
};

