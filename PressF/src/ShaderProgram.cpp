#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(std::vector<Shader* > li)
	: std::vector<Shader* >(li)
{
	Compile();
};

ShaderProgram::~ShaderProgram() {
	std::cout << "Die ShaderProgram" << std::endl;
	GLCALL(glDeleteProgram(id));
};

ShaderProgram* ShaderProgram::GetDefault(IllumModel model) {
	//ShaderProgram *a = ShaderProgram::shaders[static_cast<int>(model)];

	//if (a) {
	//	return a;
	//}

	//else {
	//	PF_WARN("DEFUALT SHADER USED");
	//	return ShaderProgram::shaders[static_cast<int>(IllumModel::COOK)];
	//}

	__debugbreak();
	throw std::exception("NOT RECOGNIZED SHADER");
}

void ShaderProgram::Compile() {
	GLCALL(id = glCreateProgram());
	//std::cout << "Loading Program with --------------------------------------------------------" << std::endl;

	for (size_t ii = 0; ii < size(); ii++)
	{
		Shader &shader = *at(ii);
		shader.Compile();
		GLCALL(glAttachShader(id, shader.id));
	}

	GLCALL(glLinkProgram(id));
	int success;

	//GLCALL(glValidateProgram(id));

	//glGetProgramiv(id, GL_VALIDATE_STATUS, &success);

	//if(!success){
	//	// lo dejo asi pa ver si es necesario implementar esto y sacar un mensaje serio
	//	std::cout<< "Error validating"<<std::endl;

	//	int length;

	//	GLCALL(glGetProgramiv(id, GL_validate_, &length));

	//	char* message = static_cast<char*>(alloca(length * sizeof(char)));

	//	GLCALL(glGetProgramInfoLog(id, length, &length, message));

	//	std::cout << "Failed to validate program :(" << std::endl;
	//	std::cout << message;

	//	__debugbreak();
	//}

	GLCALL(glGetProgramiv(id, GL_LINK_STATUS, &success));

	if (!success)
	{
		int length;

		GLCALL(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length));

		char* message = static_cast<char*>(alloca(length * sizeof(char)));

		GLCALL(glGetProgramInfoLog(id, length, &length, message));

		PF_ERROR("Failed to compile program {0} :(", at(0)->path);
		PF_ERROR(message);

		//__debugbreak();
	}

	//std::cout << "Program loaded --------------------------------------------------------" << std::endl;
}

void ShaderProgram::AddShader(Shader* shader) {
	push_back(shader);
}

void ShaderProgram::ReCompile() {
	for (int ii = 0; ii < size(); ii++)
	{
		PF_ASSERT(at(ii) && "Composing shader is null");
		Shader &shader = *this->at(ii);
		shader.ReCompile();
	}
	Compile();
}

void ShaderProgram::Use() const
{
	GLCALL(glUseProgram(id));
}

void ShaderProgram::SetUniform(const std::string &name, bool value) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniform1i(loc, (int)value));
}

void ShaderProgram::SetUniform(const std::string &name, int value) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniform1i(loc, value));
}

void ShaderProgram::SetUniform(const std::string &name, float value) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniform1f(loc, value));
}

void ShaderProgram::SetUniform(const std::string &name, const glm::vec2 &value) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniform2fv(loc, 1, &value[0]));
}

void ShaderProgram::SetUniform(const std::string &name, float x, float y) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniform2f(loc, x, y));
}

void ShaderProgram::SetUniform(const std::string &name, const glm::vec3 &value) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniform3fv(loc, 1, &value[0]));
}

void ShaderProgram::SetUniform(const std::string &name, float x, float y, float z) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniform3f(loc, x, y, z));
}

void ShaderProgram::SetUniform(const std::string &name, const glm::vec4 &value) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniform4fv(loc, 1, &value[0]));
}

void ShaderProgram::SetUniform(const std::string &name, float x, float y, float z, float w)
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniform4f(loc, x, y, z, w));
}

void ShaderProgram::SetUniform(const std::string &name, const glm::mat2 &mat) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniformMatrix2fv(loc, 1, GL_FALSE, &mat[0][0]));
}

void ShaderProgram::SetUniform(const std::string &name, const glm::mat3 &mat) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]));
}

void ShaderProgram::SetUniform(const std::string &name, const glm::mat4 &mat) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]));
}