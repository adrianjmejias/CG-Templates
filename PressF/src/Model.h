#pragma once
#include "types.h"
#include "OBJ_Loader.h"



class Material 
	: public objl::Material 
{
	unsigned int smap_kD;
};

class Model
	: public objl::Loader
{
public:
	Model();
	~Model();
};

class Mesh : 
	public objl::Mesh
{
public:
	Material mat;
	unsigned int VAO;
	unsigned int nVertex;
	unsigned int EBO;
	unsigned int VBO;
};

