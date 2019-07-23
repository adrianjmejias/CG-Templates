#include "Shader.h"

Shader::Shader()
{
}

void Shader::Compile() {
	SetFromString(src, type);
}

void Shader::ReCompile() {
	//PF_INFO("Recompiling {0}", name);
	SetFromFile(path, type);
}

Shader* Shader::FromPath(const std::string path, unsigned int type) {
	auto s = new Shader();
	s->SetFromFile(path, type);
	return s;
}

Shader* Shader::FromString(const std::string str, unsigned int type) {
	auto s = new Shader();
	s->SetFromString(str, type);
	return s;
}

void Shader::SetFromString(const std::string salsa, unsigned int t) {
	src = std::move(salsa);
	type = t;

	const char* c_str = src.data();

	if (id > 0) {
		GLCALL(glDeleteShader(id));
	}

	GLCALL(id = glCreateShader(type));

	GLCALL(glShaderSource(id, 1, &c_str, 0));

	GLCALL(glCompileShader(id));

	int success;

	GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &success));

	if (!success)
	{
		int length;

		GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		char* message = static_cast<char*>(alloca(length * sizeof(char)));

		GLCALL(glGetShaderInfoLog(id, length, &length, message));

		PF_ERROR("Failed to compile shader {0}", Shader::GetShaderName(type));
		PF_ERROR(message);
		//__debugbreak();
	}
	//std::cout << "shader compiled: " << name << std::endl;
}

void Shader::SetFromFile(std::string p, unsigned int t) {
	type = t;
	path = p;

	std::ifstream shaderFile;

	// Set exceptions for ifstream object

	try
	{
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		// Open the file
		shaderFile.open(path);
		std::stringstream shaderStream;
		// Reads the buffer content into streams
		shaderStream << shaderFile.rdbuf();
		// Close the file handler
		shaderFile.close();
		// Convert the stream into a string
		src = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER Error reading file: " << path << std::endl;
		std::cout << e.what() << std::endl;
		__debugbreak();
		return;
	}
	SetFromString(src, type);
}

Shader::~Shader() {
	GLCALL(glDeleteShader(id));
}