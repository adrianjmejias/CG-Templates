#pragma once

class Shader {
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
