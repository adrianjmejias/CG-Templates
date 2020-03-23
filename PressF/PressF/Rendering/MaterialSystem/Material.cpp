#include "PressF/pch.h"
#include "Material.h"

namespace PF
{

	Material::Material() {
	}

	void Material::Bind()
	{
		//ShaderParameter* p = new ShaderInt();

		shader->Bind();
		for (auto& sp : parameters)
		{
			sp->Bind(shader);
		}
	}
	Material& Material::operator=(objl::Material& m)
	{
		AddParameter<ShaderColor>("kA", ToVec3(m.Ka));
		AddParameter<ShaderColor>("kd", ToVec3(m.Kd));
		AddParameter<ShaderColor>("kS", ToVec3(m.Ks));
		//AddParameter<ShaderInt>("mode", 0);

		name = m.name;
		//AddParameter<ShaderVec3>("nS", m.Ns);
		//AddParameter<ShaderVec3>("nI", m.Ni);
		//AddParameter<ShaderVec3>("d", m.d);
		//d = m.d;
		//illum = m.illum;
		//map_Ka = m.map_Ka;
		//map_Kd = m.map_Kd;
		//map_Ks = m.map_Ks;
		//map_Ns = m.map_Ns;
		//map_d = m.map_d;
		//map_bump = m.map_bump;
		//AddParameter()


		//name = std::move(m.name);
		return *this;
	}
	void Material::ImGui()
	{
		Asset::ImGui();
		for (auto& sp : parameters)
		{
			sp->ImGui();
		}
	}


	Vec4 ToVec4(const objl::Vector3& v)
	{
		return Vec4{ v.X, v.Y, v.Z, 1 };
	}
	Vec3 ToVec3(const objl::Vector3& v)
	{
		return Vec3{ v.X, v.Y, v.Z };
	}

	Vec2 ToVec2(const objl::Vector2& v)
	{
		return Vec2{ v.X, v.Y };
	}


}