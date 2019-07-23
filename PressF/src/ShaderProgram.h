#pragma once
#include "types.h"
#include "Shader.h"

enum class IllumModel {
	CUBEMAP = 11,
	NORMAL,
	OCCLUSION_PARALLAX,
	REFLECTION,
	REFRACTION,
	TRANSPARENCY,
	SHADOW_MAPPING,
	COOK,
	BLINN_PHONG,
	DISPLACEMENT,
	NO_SHADER,
};

enum class MapType {
	AMBIENT,
	DIFFUSE,
	SPECULAR,
	BUMP,
	CUBEMAP,
	SHINY,
	DISPLACEMENT,
	DECAL,
	REFLECTION,
	DISSOLVE,
};

class ShaderProgram :
	protected std::vector< Shader *>
{
public:
	static ShaderProgram* shaders[30];
	unsigned int id;
	bool lit = true;
	bool viewDependant = true;
	bool MVP = true;
	bool shadows = true;
	
	static ShaderProgram* GetDefault(IllumModel model);

	ShaderProgram(std::vector<Shader* > li);

	~ShaderProgram();

	unsigned int Get();
	void ReCompile();
	void AddShader(Shader* shader);
	void Compile();
public:
	void  Use() const;
	void  SetUniform(const std::string &name, bool value) const;
	void  SetUniform(const std::string &name, int value) const;
	void  SetUniform(const std::string &name, float value) const;
	void  SetUniform(const std::string &name, const glm::vec2 &value) const;
	void  SetUniform(const std::string &name, float x, float y) const;
	void  SetUniform(const std::string &name, const glm::vec3 &value) const;
	void  SetUniform(const std::string &name, float x, float y, float z) const;
	void  SetUniform(const std::string &name, const glm::vec4 &value) const;
	void  SetUniform(const std::string &name, float x, float y, float z, float w);
	void  SetUniform(const std::string &name, const glm::mat2 &mat) const;
	void  SetUniform(const std::string &name, const glm::mat3 &mat) const;
	void  SetUniform(const std::string &name, const glm::mat4 &mat) const;

};


// forcing shader variables to be the same as how access them in code
#define SET_UNIFORM(SHADER, UNIFORM_VARIABLE)\
	SHADER.SetUniform(#UNIFORM_VARIABLE, UNIFORM_VARIABLE)\


