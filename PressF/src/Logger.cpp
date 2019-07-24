#include "Logger.h"

Logger::Logger()
{
}

Logger::~Logger()
{
}

bool GLLogCall(const char * function, const char * file, int line) {
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

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

std::ostream& operator<<(std::ostream& os, const Vec3& v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}
std::istream& operator>> (std::istream& is, Vec3& v)
{
	is >> v.x >> v.y >> v.z;
	return is;
}
