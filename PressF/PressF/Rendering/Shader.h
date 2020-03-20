#pragma once
#include "../pch.h"
#include <glad/glad.h>
//#include "Logger.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "../Logger.h"


namespace PF
{
	class ShaderProgram {
	public:
		std::string path;
		std::string src;
		unsigned int id = 0;
		int type;

		static ShaderProgram* FromString(const std::string str, unsigned int type);
		static ShaderProgram* FromPath(const std::string path, unsigned int type);
		void ReCompile();
		void Compile();
		~ShaderProgram();

	private:
		ShaderProgram();

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
}