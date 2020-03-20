#include "PressF/pch.h"
#include "types.h"



namespace Serialization
{
	json Serialize(Vec3 vec)
	{
		json j;
		j[0] = vec[0];
		j[1] = vec[1];
		j[2] = vec[2];
		return j;
	}
}