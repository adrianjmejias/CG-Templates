#pragma once
#include "../pch.h"
#include "../ObjLoader/OBJ_Loader.h"
//#include "../AssetsManagement/Asset.h"
#include "Texture.h"
namespace PF
{


	


	struct Material : public Asset
	{
	public:
		Vec3 kA{0,0,0};
		Vec3 kD{0.8f,0.8f,0.8f };
		Vec3 kS{ 0.8f,0.8f,0.8f };
		Float Ns;
		Float Ni;
		Float d;
		Float IOR = 1.2f;
		Int illum = 0;
		Ref<Texture> map_Ka;
		Ref<Texture> map_Kd;
		Ref<Texture> map_Ks;
		Ref<Texture> map_Ns;
		Ref<Texture> map_d;
		Ref<Texture> map_bump;
		Material();
		//Material(const objl::Material& m);

		//Ref<Texture> operator[](int idx)
		//{

		//	if (idx < 0 || idx >= 6) {
		//		__debugbreak();
		//	}
		//		return *static_cast<Ref<Texture>>(std::make_sharedthis + offsetof(Material ,map_Ka) + idx * sizeof(Ref<Texture>));
		//}

		Material& operator = (const objl::Material &m)
		{
			kA = m.Ka;
			kD = m.Kd;
			kS = m.Ks;
			Ns = m.Ns;	
			Ni = m.Ni;
			d = m.d;
			illum = m.illum;
			//map_Ka = m.map_Ka;
			//map_Kd = m.map_Kd;
			//map_Ks = m.map_Ks;
			//map_Ns = m.map_Ns;
			//map_d = m.map_d;
			//map_bump = m.map_bump;
			name = m.name;
			return *this;
		}
	};
}
