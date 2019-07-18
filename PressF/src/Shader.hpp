#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Asset;

class Shader : public Asset {
	std::string src;
	unsigned int id = 0;
	int type;
public:
	unsigned int Get() { return id; }

	Shader(const std::string path, unsigned int type);

	void ReCompile();

	void SetFromString(const std::string salsa, unsigned int type);

	void SetFromFile(std::string path, unsigned int type);

	~Shader();
private:
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
		return "not defined";
	}

};
