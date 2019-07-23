#pragma once
#include "types.h"

class Shader  {
public:
	std::string path;
	std::string src;
	unsigned int id = 0;
	int type;

	static Shader* FromString(const std::string str, unsigned int type);
	static Shader* FromPath(const std::string path, unsigned int type);
	void ReCompile();
	void Compile();
	~Shader();

private:
	Shader();

	void SetFromString(const std::string salsa, unsigned int type);
	void SetFromFile(std::string path, unsigned int type);

	static std::string GetShaderName(const unsigned int type) {
		switch (type)
		{
		case GL_VERTEX_SHADER:
			return "vertex";
			break;
		case GL_FRAGMENT_SHADER:
			return "fragment";
			break;
		case GL_GEOMETRY_SHADER:
			return "geometry";
			break;
		}

		__debugbreak();
		throw std::exception("Not implemented");
	}
};


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