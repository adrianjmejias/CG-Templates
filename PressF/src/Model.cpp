#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
	std::cout << "Bye Model" << std::endl;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

Material::Material(){
	name;
	Ns = 0.0f;
	Ni = 0.0f;
	d = 0.0f;
	illum = 0;
}

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

Vertex::Vertex(const objl::Vertex & v) {
	Assign(pos, v.Position);
	Assign(normal, v.Normal);
	Assign(uv, v.TextureCoordinate);
}
