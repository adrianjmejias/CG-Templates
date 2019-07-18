#include "ShaderProgram.hpp"

void ShaderProgram::Compile() {

	GLCALL(id = glCreateProgram());
	std::cout << "Loading Program with --------------------------------------------------------" << std::endl;

	for (size_t ii = 0; ii < size(); ii++)
	{
		Shader &shader = *(at(ii));
		shader.ReCompile();
		GLCALL(glAttachShader(id, shader.Get()));

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

		std::cout << "Failed to compile program :(" << std::endl;
		std::cout << message;

	}


	std::cout << "Program loaded --------------------------------------------------------" << std::endl;
}

void ShaderProgram::AddShader(std::shared_ptr<Shader> shader) {
	push_back(shader);
}

void ShaderProgram::ReCompile() {
	for (size_t ii = 0; ii < size(); ii++)
	{
		Shader &shader = *at(ii);
		shader.ReCompile();

	}
	Compile();
}

unsigned int ShaderProgram::Get() {
	return id;
}

void ShaderProgram::use()
{
	GLCALL(glUseProgram(id));
}

void ShaderProgram::setBool(const std::string &name, bool value) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniform1i(loc, (int)value));
}

void ShaderProgram::setInt(const std::string &name, int value) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniform1i(loc, value));
}

void ShaderProgram::setFloat(const std::string &name, float value) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniform1f(loc, value));
}

void ShaderProgram::setVec2(const std::string &name, const glm::vec2 &value) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniform2fv(loc, 1, &value[0]));
}

void ShaderProgram::setVec2(const std::string &name, float x, float y) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniform2f(loc, x, y));
}

void ShaderProgram::setVec3(const std::string &name, const glm::vec3 &value) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniform3fv(loc, 1, &value[0]));
}

void ShaderProgram::setVec3(const std::string &name, float x, float y, float z) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniform3f(loc, x, y, z));
}

void ShaderProgram::setVec4(const std::string &name, const glm::vec4 &value) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniform4fv(loc, 1, &value[0]));
}

void ShaderProgram::setVec4(const std::string &name, float x, float y, float z, float w)
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniform4f(loc, x, y, z, w));
}

void ShaderProgram::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniformMatrix2fv(loc, 1, GL_FALSE, &mat[0][0]));
}

void ShaderProgram::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]));
}

void ShaderProgram::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	GLCALL(unsigned int loc = glGetUniformLocation(id, name.c_str()));
	GLCALL(glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]));
}
