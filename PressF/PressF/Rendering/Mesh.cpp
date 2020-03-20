#include "PressF/pch.h"
#include "Mesh.h"


namespace PF
{
	Model::Model()
	{
	}

	Model::~Model()
	{
	}

	void Assign(Vec3& a, const objl::Vector3& b) {
		a.x = b.X;
		a.y = b.Y;
		a.z = b.Z;
	}

	void Assign(Vec2& a, const objl::Vector2& b) {
		a.x = b.X;
		a.y = b.Y;
	}

	Vertex::Vertex(const objl::Vertex& v) {
		Assign(pos, v.Position);
		Assign(normal, v.Normal);
		Assign(uv, v.TextureCoordinate);
	}
}