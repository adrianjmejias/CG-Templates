#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glad\glad.h>
#include <vector>
#include <string>
#include <map>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <tuple>
#include "Logger.h"

bool GLLogCall(const char* function, const char* file, int line);

#define GLASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
	x;\
	GLASSERT(GLLogCall(#x, __FILE__, __LINE__))\

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::string errName = "";
		switch (error)
		{
		case(0x0500):
			errName = "GL_INVALID_ENUM";
			break;
		case(0x0501):
			errName = "GL_INVALID_VALUE";
			break;
		case(0x0502):
			errName = "GL_INVALID_OPERATION";
			break;
		case(0x0505):
			errName = "GL_OUT_OF_MEMORY";
			break;
		default:break;
		}

		PF_ERROR("[OpenGL Error {0} | {1}  \nCall {2}  \nError : {3}]", file, line, function, errName);
		return false;
	}

	return true;
}

#include "Window.h"
#include "UserInterface.h"

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
