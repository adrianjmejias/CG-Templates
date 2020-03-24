#include "PressF/pch.h"
#include "Material.h"

namespace PF
{

	void Material::Bind()
	{
		//ShaderParameter* p = new ShaderInt();

		shader->Bind();
		for (auto& sp : parameters)
		{
			sp->Bind(shader);
		}
	}

	void Material::BindParametersOnly(Ref<ShaderProgram> s)
	{
		if (s == nullptr)
		{
			s = shader;
		}
		//ShaderParameter* p = new ShaderInt();
		for (auto& sp : parameters)
		{
			sp->Bind(s);
		}
	}
	Material& Material::operator=(objl::Material& m)
	{
		name = m.name;

		AddParameter<ShaderColor>("kA", ToVec3(m.Ka));
		AddParameter<ShaderColor>("kD", ToVec3(m.Kd));
		AddParameter<ShaderColor>("kS", ToVec3(m.Ks));

		/*
		-- Ns
		Specifies the specular exponent for the current material.  This defines 
		the focus of the specular highlight.
 
		 "exponent" is the value for the specular exponent.  A high exponent 
		results in a tight, concentrated highlight.  Ns values normally range 
		from 0 to 1000.
		*/
		AddParameter<ShaderFloat>("nS", Float{ m.Ns });
		AddParameter<ShaderFloat>("roughness", Float{ 1 });

		

		//  -- Ni optical_density

		//	Specifies the optical density for the surface.This is also known as
		//	index of refraction.

		//	"optical_density" is the value for the optical density.The values can
		//	range from 0.001 to 10.  A value of 1.0 means that light does not bend
		//	as it passes through an object.Increasing the optical_density
		//	increases the amount of bending.Glass has an index of refraction of
		//	about 1.5.Values of less than 1.0 produce bizarre results and are not
		//	recommended.

		AddParameter<ShaderFloat>("nI", Float{ m.Ni });




		//AddParameter<ShaderFloat>("d", m.d);


		//d = m.d;
		//illum = m.illum;
		//map_Ka = m.map_Ka;
		//map_Kd = m.map_Kd;
		//map_Ks = m.map_Ks;
		//map_Ns = m.map_Ns;
		//map_d = m.map_d;
		//map_bump = m.map_bump;


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