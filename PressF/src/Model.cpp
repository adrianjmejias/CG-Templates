#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
}

//Vertex::Vertex(objl::Vertex v)
//{
//
//}

Material::Material(const objl::Material& m) {
	*this = m;
}

void Assign(Vec3 & a, const objl::Vector3 & b) {
	a.x = b.X;
	a.y = b.Y;
	a.z = b.Z;
}

void Assign(Vec2 & a, const objl::Vector2 & b) {
	a.x = b.X;
	a.y = b.Y;
}