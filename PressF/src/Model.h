#pragma once
#include "pch.h"
#include "Physics.h"
#include "OBJ_Loader.h"
#include "Texture.h"
class MeshRenderer;
void Assign(Vec3& a, const objl::Vector3& b);
void Assign(Vec2& a, const objl::Vector2& b);

class Vertex {
public:
	Vec3 pos;
	Vec3 normal;
	Vec2 uv;
	Vec3 tan;
	Vertex(const objl::Vertex& v);
};



class Material 
{
public:
	std::string name;
	Vec3 kA{0,0,0};
	Vec3 kD{0.8f,0.8f,0.8f };
	Vec3 kS{ 0.8f,0.8f,0.8f };
	float Ns;
	float Ni;
	float d;
	float IOR = 1.2f;
	int illum = 0;
	std::string map_Ka;
	std::string map_Kd;
	std::string map_Ks;
	std::string map_Ns;
	std::string map_d;
	std::string map_bump;
	Texture* smap_Ka = nullptr;
	Texture* smap_Kd = nullptr;
	Texture* smap_Ks = nullptr;
	Texture* smap_Ns = nullptr;
	Texture* smap_d = nullptr;
	Texture* smap_bump = nullptr;
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
		name = m.name;
		return *this;
	}
};

class Mesh  
	: public std::vector<MeshRenderer*>
{
public:
	std::string name;
	unsigned int VAO = 0, EBO= 0, VBO=0;
	GLsizei offset, nElem;
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

