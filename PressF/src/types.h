#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glad\glad.h>

#include "Logger.h"
#include "Window.h"
#include "UserInterface.h"




#include <vector>
#include <string>
#include <map>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <fstream>



#define STL_BYTE_SIZE(cont, type) (cont.size() * sizeof(type))


using Color = glm::vec4;
using Vec3 = glm::vec3;
using Vec2 = glm::vec2;
using Vec4 = glm::vec4;
using iVec3 = glm::ivec3;
using Mat4 = glm::mat4;


//std::ostream& operator<<(std::ostream& os, const Vec3& v)
//{
//	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
//	return os;
//}
//std::istream& operator>> (std::istream& is, Vec3& v)
//{
//	is >> v.x >> v.y >> v.z;
//	return is;
//}