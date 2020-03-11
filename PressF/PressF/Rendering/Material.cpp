#include "Material.h"



namespace PF
{

	Material::Material() {
		name;
		Ns = 0.0f;
		Ni = 0.0f;
		d = 0.0f;
		illum = 0;
	}

	//Material::Material(const objl::Material& m) {
	//	*this = m;
	//}
}