#pragma once

#pragma region Includes

#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_opengl.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>


#define PF_INFO(...)      spdlog::info(__VA_ARGS__)
#define PF_ERROR(...)     spdlog::error(__VA_ARGS__)
#define PF_DEBUG(...)     spdlog::debug(__VA_ARGS__)
#define PF_WARN(...)      spdlog::warn(__VA_ARGS__)
#define PF_CRITICAL(...)  spdlog::critical(__VA_ARGS__)


#ifdef  DEBUG
#define PF_ASSERT(x) if(!(x)){\
	PF_ERROR("PF_ASSERT FAILED : {0}" , #x);\
	PF_ERROR("LINE : {0}", __LINE__);\
	PF_ERROR("FILE : {0}", __FILE__);\
		__debugbreak();\
	}\

//TODO: PONER ELSE PARA PROD BUILD

#endif //  DEBUG


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


/* nuklear - 1.32.0 - public domain */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <time.h>



#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#include <nuklear.h>
#include "nuklear_sdl_gl3.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

/* ===============================================================
*
*                          EXAMPLE
*
* ===============================================================*/
/* This are some code examples to provide a small overview of what can be
* done with this library. To try out an example uncomment the defines */
//#define INCLUDE_ALL 
/*#define INCLUDE_STYLE */
#define INCLUDE_OVERVIEW

/*#define INCLUDE_CALCULATOR */
/*#define INCLUDE_NODE_EDITOR */

#ifdef INCLUDE_ALL
#define INCLUDE_STYLE
#define INCLUDE_CALCULATOR
#define INCLUDE_OVERVIEW
#define INCLUDE_NODE_EDITOR
#endif

#ifdef INCLUDE_STYLE
#include "style.c"
#endif
#ifdef INCLUDE_CALCULATOR
#include "calculator.c"
#endif
#ifdef INCLUDE_OVERVIEW
#include "overview.c"
#endif
#ifdef INCLUDE_NODE_EDITOR
#include "node_editor.c"
#endif


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <iostream>
#include <sstream>
#include <fstream>
#include <initializer_list>

//#include <filesystem>

#pragma endregion Includes

#define COMP_PARAMS GameObject &o, Transform &t
#define INIT_COMP(n) :Component(o,t, n)

/* Platform */
SDL_Window *win;
SDL_GLContext glContext;
int win_width, win_height;
int running = 1;

/* GUI */

struct nk_context *ctx;
struct nk_colorf bg;

Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;
double deltaTime = 0;

#pragma region Types

using Color = nk_colorf;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using iVec3 = glm::ivec3;

std::istream& operator>> (std::istream& is, Vec3& v)
{
	is >> v.x >> v.y >> v.z;
	return is;
}
std::istream& operator>> (std::istream& is, iVec3& v)
{
	is >> v.x >> v.y >> v.z;
	return is;
}
using Mat4 = glm::mat4;

enum class IllumModel {
	CUBEMAP = 11,
	REFLECTION,
	REFRACTION,
	SHADOW,
	COOK,
	BLINN_PHONG
};


class SystemRenderer;
class GameObject;
class Component;
class Texture;
class Camera;
class Asset;
class Transform;
class GameObject;
class Mesh;


#pragma endregion Types


class AssetManager {
public:
	std::list<Asset> assets;
};

class Asset {
public:

	Asset(const std::string n) : name(n)
	{
	}
	//Asset(const std::filesystem::path p): name(p.filename)
	//{

	//	path = p;
	//}
	std::string name;
	std::string path;

};


#pragma region Shading

class Shader : public Asset {
	std::string src;
	unsigned int id = 0;
	int type;
	Shader() : Asset("Shader") {}
public:
	unsigned int Get() { return id; }


	static Shader* FromString(const std::string str, unsigned int type) {
		auto s = new Shader();
		s->SetFromString(str, type);
		return s;
	}
	static Shader* FromPath(const std::string path, unsigned int type) {
		auto s = new Shader();
		s->SetFromFile(path, type);
		return s;
	}

	void ReCompile();
	void Compile() {
		SetFromString(src, type);
	}
private:
	void SetFromString(const std::string salsa, unsigned int type);

	void SetFromFile(std::string path, unsigned int type);

public:
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

class ShaderProgram :
	protected std::vector< Shader *>
{
private:
	unsigned int id;
public:


	static ShaderProgram* GetDefault(IllumModel model) {

		switch (model) {
		case IllumModel::CUBEMAP:
			return new ShaderProgram({
				Shader::FromPath("../../assets/shaders/defaults/CUBEMAP.vert", GL_VERTEX_SHADER),
				Shader::FromPath("../../assets/shaders/defaults/CUBEMAP.frag", GL_FRAGMENT_SHADER),
				});

		case IllumModel::REFLECTION:
			return new ShaderProgram({
				Shader::FromPath("REFLECTION.vert", GL_VERTEX_SHADER),
				Shader::FromPath("REFLECTION.frag", GL_FRAGMENT_SHADER),
				});
		case IllumModel::REFRACTION:
			return new ShaderProgram({
				Shader::FromPath("REFRACTION.vert", GL_VERTEX_SHADER),
				Shader::FromPath("REFRACTION.frag", GL_FRAGMENT_SHADER),
				});
		case IllumModel::SHADOW:
			return new ShaderProgram({
				Shader::FromPath("SHADOW.vert", GL_VERTEX_SHADER),
				Shader::FromPath("SHADOW.frag", GL_FRAGMENT_SHADER),
				});
		case IllumModel::COOK:
			return new ShaderProgram({
				Shader::FromPath("COOK.vert", GL_VERTEX_SHADER),
				Shader::FromPath("COOK.frag", GL_FRAGMENT_SHADER),
				});
		case IllumModel::BLINN_PHONG:
			return new ShaderProgram({
				Shader::FromPath("BLINN_PHONG.vert", GL_VERTEX_SHADER),
				Shader::FromPath("BLINN_PHONG.frag", GL_FRAGMENT_SHADER),
				});
		}




		__debugbreak();
		throw std::exception("NOT RECOGNIZED SHADER");
	}



	ShaderProgram(std::vector<Shader* > li) : std::vector<Shader* >(std::move(li))
	{
		Compile();
	};

	~ShaderProgram() {
		std::cout << "Die ShaderProgram" << std::endl;
		GLCALL(glDeleteProgram(id));
	};
	unsigned int Get();
	void ReCompile();
	void AddShader(Shader* shader);
	void Compile();
public:
	/**
	* Enables the shader to be use
	*/
	void  use();

	/**
	* Sets a bool uniform
	* @param{std::string &} uniform name
	* @param{bool} value to be set
	*/
	void  setBool(const std::string &name, bool value) const;

	/**
	* Sets an int uniform
	* @param{std::string &} uniform name
	* @param{int} value to be set
	*/
	void  setInt(const std::string &name, int value) const;

	/**
	* Sets an int uniform
	* @param{std::string &} uniform name
	* @param{float} value to be set
	*/
	void  setFloat(const std::string &name, float value) const;

	/**
	* Sets an vec2 uniform
	* @param{std::string &} uniform name
	* @param{vec2} value to be set
	*/
	void  setVec2(const std::string &name, const glm::vec2 &value) const;

	/**
	* Sets an vec2 uniform
	* @param{std::string &} uniform name
	* @param{float} vec2 x value
	* @param{float} vec2 y value
	*/
	void  setVec2(const std::string &name, float x, float y) const;

	/**
	* Sets an vec3 uniform
	* @param{std::string &} uniform name
	* @param{vec3} vector value
	*/
	void  setVec3(const std::string &name, const glm::vec3 &value) const;

	/**
	* Sets an vec3 uniform
	* @param{std::string &} uniform name
	* @param{float} vec3 x value
	* @param{float} vec3 y value
	* @param{float} vec3 z value
	*/
	void  setVec3(const std::string &name, float x, float y, float z) const;

	/**
	* Sets an vec4 uniform
	* @param{std::string &} uniform name
	* @param{vec4} vec4 value
	*/
	void  setVec4(const std::string &name, const glm::vec4 &value) const;

	/**
	* Sets an vec4 uniform
	* @param{std::string &} uniform name
	* @param{float} vec4 x value
	* @param{float} vec4 y value
	* @param{float} vec4 z value
	* @param{float} vec4 w value
	*/
	void  setVec4(const std::string &name, float x, float y, float z, float w);

	/**
	* Sets an mat2 uniform
	* @param{std::string &} uniform name
	* @param{mat2} mat2 value
	*/
	void  setMat2(const std::string &name, const glm::mat2 &mat) const;

	/**
	* Sets an mat3 uniform
	* @param{std::string &} uniform name
	* @param{mat3} mat3 value
	*/
	void  setMat3(const std::string &name, const glm::mat3 &mat) const;

	/**
	* Sets an mat4 uniform
	* @param{std::string &} uniform name
	* @param{mat4} mat4 value
	*/
	void  setMat4(const std::string &name, const glm::mat4 &mat) const;
};

void Shader::ReCompile() {
	SetFromFile(path, type);
}

void Shader::SetFromString(const std::string salsa, unsigned int type) {
	src = std::move(salsa);
	this->type = type;

	const char* c_str = src.data();

	if (id)
		GLCALL(glDeleteShader(id));

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

		std::cout << "Failed to compile shader" << Shader::GetShaderName(type) << std::endl;
		std::cout << message;

	}
	std::cout << "shader compiled: " << name << std::endl;
}


void Shader::SetFromFile(std::string path, unsigned int type) {
	name = path;
	this->path = path;
	std::string shaderCode;
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
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER Error reading file: " << path << std::endl;
		std::cout << e.what() << std::endl;
		return;
	}
	SetFromString(shaderCode, type);
}

Shader::~Shader() {
	GLCALL(glDeleteShader(id));
}












void ShaderProgram::Compile() {

	GLCALL(id = glCreateProgram());
	std::cout << "Loading Program with --------------------------------------------------------" << std::endl;

	for (size_t ii = 0; ii < size(); ii++)
	{
		Shader &shader = *at(ii);
		shader.Compile();
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


void ShaderProgram::AddShader(Shader* shader) {
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

#pragma endregion Shading




std::vector<GameObject*> objects;


class Component {
public:
	std::string name;
	int openUI = true;
	int enabled = true;
	Transform &transform;
	GameObject &gameobject;

	Component(GameObject &o, Transform &t, const std::string n) : transform(t), gameobject(o)
	{
		name = n;
	}

	virtual void UI() = 0;
	virtual void Update() = 0;
	virtual void HandleEvent(const SDL_Event &e) = 0;
};

/*
	This is important to know how se are updating the accumulated matrix
	Dirty::Acum -> if my parent model changed, I just have to update my accumulated and pass the change to my children.
	Dirty::Model -> if my model changed, I have to update my model, update the accumulated and pass the change to my children.

	they are ordered according to dirtyness. don't move future Adrian.
*/
enum class Dirty {
	None,
	Acum,
	Model,
};

class Transform {
public:
	Transform(GameObject& go) :
		gameobject(go)
	{

	}
	Transform *parent = nullptr;
	std::vector<Transform *> children;

	GameObject &gameobject;
	Vec3 position{ 0, 0, 0 };
	Vec3 rotation{ 0, 0, 0 };
	Vec3 scale{ 1, 1, 1 };
	Mat4 model;
	Mat4 acum;
	Dirty dirty = Dirty::None;

#pragma region mutators

	Vec3 GetRotation() { return rotation; }
	Vec3 GetScale() { return scale; }
	Vec3 GetPosition() { return position; }
	Transform* SetRotation(const Vec3& val) { SetDirty(Dirty::Model);  rotation = val; return this; }
	Transform* SetScale(const Vec3& val) { SetDirty(Dirty::Model); scale = val; return this; }
	Transform* SetPosition(const Vec3& val) { SetDirty(Dirty::Model); position = val; return this; }
	Transform* SetRotation(float x, float y, float z) { return SetRotation(Vec3(x, y, z)); }
	Transform* SetScale(float x, float y, float z) { return SetScale(Vec3(x, y, z)); }
	Transform* SetPosition(float x, float y, float z) { return SetPosition(Vec3(x, y, z)); }
	Transform* Translate(const Vec3& val) { return SetPosition(position + val); }
	Transform* Rotate(const Vec3& val) { return SetRotation(rotation + val); }
	Transform* Scale(const Vec3& val) { return SetScale(scale + val); }
	Transform* Translate(float x, float y, float z) { return Translate(Vec3(x, y, z)); }
	Transform* Rotate(float x, float y, float z) { return Rotate(Vec3(x, y, z)); }
	Transform* Scale(float x, float y, float z) { return Scale(Vec3(x, y, z)); }
#pragma endregion mutators

	/*
	we only update if we are getting dirtier ;)
	*/
private:
	Transform * SetDirty(Dirty newVal) {
		if (static_cast<int>(dirty) < static_cast<int>(newVal)) {
			dirty = newVal;
		}
		return this;
	}


	/*
	Returs true if it was dirty.
	Cleans accumulated matrix
	*/
	bool TryGetClean() {
		if (dirty == Dirty::None) return false;

		if (dirty == Dirty::Model) {
			model = Transform::GenModel(scale, position, rotation);
			dirty = Dirty::Acum; // IMPORTANTEEEEEE SINO LOS HIJOS NO SE ACTUALIZAN
		}

		if (dirty == Dirty::Acum) {
			if (parent == nullptr) {
				acum = model;
			}
			else
			{
				acum = parent->GetAccumulated() * model;
			}

			for (auto child : children) {
				child->SetDirty(Dirty::Acum);
			}
		}

		dirty = Dirty::None;
		return true;
	}

public:
	Transform * SetParent(Transform *other) {
		if (!other) {
			// poner como root node porque estoy es quitando el padre
			throw std::exception("Not implemented yet");
			return this;
		}

		if (parent == other) {
			return this; // it is done already
		}

		if (other->parent == this) {
			throw std::exception("CICLO INFINITO POR GAFO EN LA JERARQUíA\n");
		}

		//ponerme a mi de padre
		SetDirty(Dirty::Acum);

		parent = other; // el es mi padre

		other->children.push_back(this);// yo soy su hijo

		return this;
	}
	Mat4& GetAccumulated() {
		TryGetClean();
		return acum;
	}
	Mat4& GetModel() {
		TryGetClean();
		return model;
	}



	static Mat4 GenModel(const Vec3 &scale, const Vec3 &position, const Vec3 &rotation) {
		Mat4 model = Mat4(1);
		model = glm::scale(model, scale);
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(rotation.x), Vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(rotation.y), Vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(rotation.z), Vec3(0, 0, 1));
		return model;
	}

};

class GameObject {
	friend class SystemRenderer;
	std::list<Component*> components;

public:
	int openUI = false;
	Transform transform;
	std::string name;
	GameObject(std::string n) : transform(*this) {
		name = n;
	}
	void Update() {
		for each (auto comp in components)
		{
			PF_ASSERT(comp && "COMPONENT IS NULL");

			PF_INFO("Object {0}", name);
			if (comp->enabled) {
				comp->Update();
			}
		}
	}


	void HandleEvent(const SDL_Event &e) {
		for each (auto comp in components)
		{
			PF_ASSERT(comp && "COMPONENT IS NULL");
			if (comp->enabled) {

				//PF_INFO("HANDLE EVENT ,{0}", this->name);
				comp->HandleEvent(e);
			}
		}
	}
	void UI() {
		if (nk_tree_push(ctx, NK_TREE_TAB, name.data(), static_cast<nk_collapse_states>(openUI))) {

			for each (auto comp in components)
			{
				PF_ASSERT(comp && "component is null");

				if (nk_tree_push(ctx, NK_TREE_TAB, comp->name.data(), static_cast<nk_collapse_states>(comp->openUI))) {

					nk_checkbox_label(ctx, "Enabled", &comp->enabled);

					comp->UI();

					nk_tree_pop(ctx);
				}

			}

			nk_tree_pop(ctx);
		}
	}

	template <typename TT, typename ...Args>
	TT& AddComponent(Args&&... params) {
		TT* comp = new TT(*this, transform, std::forward<Args>(params)...);
		components.push_back(comp);
		return *comp;
	}
};

enum class FBAttachment {
	COLOR_ATTACHMENT0 = GL_COLOR_ATTACHMENT0,
	COLOR_ATTACHMENT1 = GL_COLOR_ATTACHMENT1,
	COLOR_ATTACHMENT2 = GL_COLOR_ATTACHMENT2,
	COLOR_ATTACHMENT3 = GL_COLOR_ATTACHMENT3,
	COLOR_ATTACHMENT4 = GL_COLOR_ATTACHMENT4,
	COLOR_ATTACHMENT5 = GL_COLOR_ATTACHMENT5,
	COLOR_ATTACHMENT6 = GL_COLOR_ATTACHMENT6,
	COLOR_ATTACHMENT7 = GL_COLOR_ATTACHMENT7,
	COLOR_ATTACHMENT8 = GL_COLOR_ATTACHMENT8,
	COLOR_ATTACHMENT9 = GL_COLOR_ATTACHMENT9,
	COLOR_ATTACHMENT10 = GL_COLOR_ATTACHMENT10,
	COLOR_ATTACHMENT11 = GL_COLOR_ATTACHMENT11,
	COLOR_ATTACHMENT12 = GL_COLOR_ATTACHMENT12,
	COLOR_ATTACHMENT13 = GL_COLOR_ATTACHMENT13,
	COLOR_ATTACHMENT14 = GL_COLOR_ATTACHMENT14,
	COLOR_ATTACHMENT15 = GL_COLOR_ATTACHMENT15,
	COLOR_ATTACHMENT16 = GL_COLOR_ATTACHMENT16,
	COLOR_ATTACHMENT17 = GL_COLOR_ATTACHMENT17,
	COLOR_ATTACHMENT18 = GL_COLOR_ATTACHMENT18,
	COLOR_ATTACHMENT19 = GL_COLOR_ATTACHMENT19,
	COLOR_ATTACHMENT20 = GL_COLOR_ATTACHMENT20,
	COLOR_ATTACHMENT21 = GL_COLOR_ATTACHMENT21,
	COLOR_ATTACHMENT22 = GL_COLOR_ATTACHMENT22,
	COLOR_ATTACHMENT23 = GL_COLOR_ATTACHMENT23,
	COLOR_ATTACHMENT24 = GL_COLOR_ATTACHMENT24,
	COLOR_ATTACHMENT25 = GL_COLOR_ATTACHMENT25,
	COLOR_ATTACHMENT26 = GL_COLOR_ATTACHMENT26,
	COLOR_ATTACHMENT27 = GL_COLOR_ATTACHMENT27,
	COLOR_ATTACHMENT28 = GL_COLOR_ATTACHMENT28,
	COLOR_ATTACHMENT29 = GL_COLOR_ATTACHMENT29,
	COLOR_ATTACHMENT30 = GL_COLOR_ATTACHMENT30,
	COLOR_ATTACHMENT31 = GL_COLOR_ATTACHMENT31,
	DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT,
	STENCIL_ATTACHMENT = GL_STENCIL_ATTACHMENT,
};


#pragma region Components
class Camera : public Component {
	friend class GameObject;
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 5.f;
	const float SENSITIVITY = 1.f;
	const float ZOOM = 45.0f;

	unsigned int power = 10000;
	bool isPerspective = true;
	float fov = 45;
	float nearClippingPlane = 0.1f;
	float farClippingPlane = 200.0f;


	Mat4 projection;
	Mat4 view;


	Camera(COMP_PARAMS) INIT_COMP("Camera")
	{

	}

public:
	// Inherited via Component
	virtual void Update() override {
		//PF_INFO("Camera running");
	}


	// Inherited via Component
	virtual void UI() override {
		fov = nk_propertyf(ctx, "FOV", 30.f, fov, 120.0f, 0.01f, 0.005f);
	}

	virtual void HandleEvent(const SDL_Event &e) override {
		if (e.type == SDL_EventType::SDL_KEYDOWN) {
			if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_W) {
				PF_INFO(" Camera SDL_SCANCODE_W");

			}
			if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_S) {
				PF_INFO(" Camera SDL_SCANCODE_S");

			}
			if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_A) {
				PF_INFO(" Camera SDL_SCANCODE_A");

			}
			if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_D) {
				PF_INFO(" Camera SDL_SCANCODE_D");

			}
		}
	}
	virtual Mat4& GetView() {




		return view;
	}
	virtual Mat4& GetProjection() {


		return view;
	}


};

class Mover : public Component {

};


class Renderer : public Component {

public:
	virtual void Render() = 0;
};

class MeshRenderer : public Renderer {
public:
	Mesh &mesh;



};



#pragma region Light
enum class LightType {
	POINT,
	DIRECTIONAL,
	SPOTLIGHT
};

#define LIGHT_PARAMS COMP_PARAMS
#define INIT_LIGHT(n) : Light(o,t,n)

class Light : public Component {
public:
	LightType type;
	Color kA{ 1,1,1,1 };
	Vec3 kD{ 0,0,1 };
	Vec3 kS{ 1,0,0 };
	Vec3 kE{ 0,0,0 };


	virtual void Bind() = 0;


	virtual void UI() override {


		if (nk_combo_begin_color(ctx, nk_rgb_cf(kA), nk_vec2(200, 400))) {

			nk_layout_row_dynamic(ctx, 120, 1);
			kA = nk_color_picker(ctx, kA, NK_RGBA);
			nk_layout_row_dynamic(ctx, 25, 2);
			nk_layout_row_dynamic(ctx, 25, 1);

			kA.r = nk_propertyf(ctx, "#R:", 0, kA.r, 1.0f, 0.01f, 0.005f);
			kA.g = nk_propertyf(ctx, "#G:", 0, kA.g, 1.0f, 0.01f, 0.005f);
			kA.b = nk_propertyf(ctx, "#B:", 0, kA.b, 1.0f, 0.01f, 0.005f);
			kA.a = nk_propertyf(ctx, "#A:", 0, kA.a, 1.0f, 0.01f, 0.005f);
			nk_combo_end(ctx);
		}
	}
	virtual void HandleEvent(const SDL_Event &e) override {}
	virtual void ShadowPass() {}

protected:
	Light(COMP_PARAMS, std::string n) INIT_COMP(n)
	{

	}
};

class PointLight : public Light {
public:
	Vec3 attenuation;

	PointLight(LIGHT_PARAMS) : Light(o, t, "PointLight")
	{
		this->type = LightType::POINT;
	}
	// Inherited via Light
	virtual void Update() override {
		//PF_INFO("PointLight running");
	}

	virtual void Bind() override {
		PF_INFO("Bind PointLight");

	}
	virtual void UI() override {
		Light::UI();
	}
};

class SpotLight : public Light {
public:
	Vec3 attenuation;
	float innerAngle = 15.f;
	float outterAngle = 20.f;
	SpotLight(LIGHT_PARAMS) : Light(o, t, "SpotLight")
	{
		this->type = LightType::SPOTLIGHT;

	}
	// Inherited via Light
	virtual void Update() override {
		//PF_INFO("SpotLight running");

	}

	virtual void Bind() override {
		PF_INFO("Bind SpotLight");
	}

	virtual void UI() override {
		Light::UI();
		innerAngle = nk_propertyf(ctx, "innerAngle", 0, innerAngle, outterAngle, 0.01f, 0.005f);
		outterAngle = nk_propertyf(ctx, "outterAngle", innerAngle, outterAngle, 180, 0.01f, 0.005f);
	}
};

class DirectionalLight : public Light {
public:
	DirectionalLight(LIGHT_PARAMS) : Light(o, t, "DirectionalLight")
	{
		this->type = LightType::DIRECTIONAL;

	}
	// Inherited via Light
	virtual void Update() override {
		//PF_INFO("DirectionalLight running");


	}
	virtual void Bind() override {
		PF_INFO("Bind DirectionalLight");
	}
	virtual void UI() override {
		Light::UI();
	}
};

#pragma endregion Light


#pragma endregion Components
class CubeMap;

#pragma region Models
//#define DEBUG_OBJ_LOADER
#ifdef DEBUG_OBJ_LOADER
#define DEBUG_PRINT(x) 
#else
#define DEBUG_PRINT(x) 
#endif // DEBUG

using Face = iVec3;

class Vertex {
public:
	Vec3 pos;
	Vec3 uv;
	Vec3 normal;
	Vec3 tangent;
	Vec3 bitangent;
};

#pragma region Texture

enum class ClampType {
	REPEAT = GL_REPEAT,
	MIRROR_REPEAT = GL_MIRRORED_REPEAT,
	EDGE = GL_CLAMP_TO_EDGE,
	BORDER = GL_CLAMP_TO_BORDER,
};

enum class TexInterpolation {
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR,
};

enum class MapType {
	AMBIENT,
	DIFFUSE,
	SPECULAR,
	SHINY,
	DISPLACEMENT,
	DECAL,
	BUMP,
	REFLECTION,
	DISSOLVE,
	CUBEMAP,
};

class Texture {
	//int width, height, nrChannels;
	//unsigned char *data;
	//for (GLuint i = 0; i < textures_faces.size(); i++)
	//{
	//	data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
	//	glTexImage2D(
	//		GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
	//		0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
	//	);
	//}
public:

	Texture() {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, static_cast<int>(clamp), static_cast<int>(texInterpolation));
		glTexParameteri(GL_TEXTURE_2D, static_cast<int>(clamp), static_cast<int>(texInterpolation));
	}

	~Texture() {
		glDeleteTextures(1, &id);
	}

	unsigned int id;


	int width;
	int height;
	int nChannels;
	ClampType clamp = ClampType::REPEAT;
	TexInterpolation texInterpolation = TexInterpolation::LINEAR;
	MapType type = MapType::AMBIENT;
	float borderColor[4] = { 1,1,1,1 };
	Texture(const std::string path, MapType t)
		: type(t)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, static_cast<int>(clamp), static_cast<int>(texInterpolation));
		glTexParameteri(GL_TEXTURE_2D, static_cast<int>(clamp), static_cast<int>(texInterpolation));

		// Creates the texture on GPU
		glGenTextures(1, &id);
		// Loads the texture
		// Flips the texture when loads it because in opengl the texture coordinates are flipped
		stbi_set_flip_vertically_on_load(true);
		// Loads the texture file data
		unsigned char *data = stbi_load(path.data(), &width, &height, &nChannels, 0);
		if (data)
		{
			// Gets the texture channel format
			GLenum format;
			switch (nChannels)
			{
			case 1:
				format = GL_RED;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			}

			// Binds the texture
			glBindTexture(GL_TEXTURE_2D, id);
			// Creates the texture
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			// Creates the texture mipmaps
			glGenerateMipmap(GL_TEXTURE_2D);

			// Set the filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<int>(clamp));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<int>(clamp));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(texInterpolation));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(texInterpolation));

		}
		else
		{
			PF_ERROR("ERROR:: Unable to load texture ", path);
			glDeleteTextures(1, &id);
		}
		// We dont need the data texture anymore because is loaded on the GPU
		stbi_image_free(data);
		PF_INFO("texture loaded {0}", path);
	}

	Texture(const std::vector<std::string> faces) {
		type = MapType::CUBEMAP;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	}


};

class CubeMap {
public:
	Texture * tex;
	ShaderProgram *shader;
	std::vector<float> skyboxVertices{
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	unsigned int skyboxVAO, skyboxVBO;

	CubeMap(const std::vector<std::string> faces) {
		shader = ShaderProgram::GetDefault(IllumModel::CUBEMAP);
		tex = new Texture(faces);


		// skybox VAO
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) *skyboxVertices.size(), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	}

	void Render(SystemRenderer &renderer) {

		shader->use();

		shader->setInt("skybox", 0);

		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

		shader->setMat4("view", renderer.camera.front().GetView());
		shader->setMat4("projection", renderer.camera.front().GetProjection());
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, tex.id);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default


	}

};


class SystemRenderer {
private:
public:
	std::list<Light*> lights;
	std::list<Camera*> camera;
	std::list<Renderer*> renderers;
	CubeMap *cubemap = nullptr;
public:
	void Steal(std::vector<GameObject*> gos) {

		for each (GameObject* go in gos)
		{
			auto &comps = go->components;

			for (auto ii = comps.begin(); ii != comps.end(); ii++)
			{
				if (Light *light = dynamic_cast<Light*>((*ii))) {
					lights.push_back(light);
					continue;
				}

				if (Camera *cam = dynamic_cast<Camera*>((*ii))) {
					camera.push_back(cam);
					continue;
				}

				if (Renderer *ren = dynamic_cast<Renderer*>((*ii))) {
					renderers.push_back(ren);
					continue;
				}


			}
		}

	}

	void Render() {
		//lights

		for each (Light* light in lights)
		{
			light->ShadowPass();
		}



		for each (Renderer* ren in renderers)
		{

			//SystemRenderer::get

			////material->bind();
			//	//light->bind();
			//ren->Render();
		}



		cubemap->Render(*this);
	}
};


#pragma endregion Texture

class SubMesh : public Asset {
public:
	Mesh & group;
	SubMesh(Mesh& g, std::string n)
		: group(g), Asset(n)
	{
	}
};

std::istream& operator>> (std::istream& file, SubMesh& mesh) {
	Vec3 v;
	Vec3 vt;
	Vec3 vn;

	iVec3 f;
	iVec3 ft;
	iVec3 fn;
	std::string tofData;


	while (!file.eof() && file.good() && file.peek() != 'o')
	{
		std::string buffer;
		std::getline(file, buffer);
		std::stringstream stream(buffer);

		stream >> tofData;

		std::cout << buffer << std::endl;
		if (tofData[0] == 'v')
		{

			if (tofData.size() > 1) {
				switch (tofData[1])
				{
				case 'n':
				{
					stream >> vn;
					//mesh->vertexNormal.push_back(vn);
					//vertexData.push_back(vn);
					DEBUG_PRINT(std::cout << "(" << vn[0] << ", " << vn[1] << ", " << vn[2] << ")" << std::endl);

				}

				break;

				case 't':
					// while(!stream.eof()) // por ahora no triangula
				{
					stream >> vt[0] >> vt[1];
					v[2] = 0;
					//mesh->vertex.push_back(vt);
					//vertexData.push_back(Vec3(vt[0], vt[1], 0.f));
					DEBUG_PRINT(std::cout << "(" << vt[0] << ", " << vt[1] << ")" << std::endl);
				}


				break;

				default:
					break;
				}
			}
			else
			{
				// Position
				// while(!stream.eof()) //por ahora no triangula
				{
					DEBUG_PRINT(std::cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")" << std::endl);
				}
			}
		}
		else if (tofData == "f")
		{
			auto posInit = stream.tellg();
			stream >> buffer;
			stream.seekg(posInit); // hago como si no hubiera leido nada del stream

								   //x case f v1 v2 v3 ....
								   //x case f v1/vt1 v2/vt2 v3/vt3 ...
								   // case f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
								   // case f v1//vn1 v2//vn2 v3//vn3 ...

			bool hasDoubleSlash = false;
			unsigned int countDeSlash = 0;
			{
				unsigned int slashPos = buffer.find_first_of("/");
				hasDoubleSlash = (buffer[slashPos + 1]) == '/';
			}
			countDeSlash = std::count(buffer.begin(), buffer.end(), '/');

			DEBUG_PRINT(std::cout << hasDoubleSlash << " " << countDeSlash << std::endl);

			//std::vector<std::string> tokens;




			switch (countDeSlash)
			{
			case 0: // solo caras
			{

				stream >> f;

				//mesh->face.push_back(f);
				DEBUG_PRINT(std::cout << "(" << f[0] << ", " << f[1] << ", " << f[2] << ")" << std::endl);
			}
			break;
			case 1: // caras y texturas
			{
				for (int ii = 0; ii < 3; ii++)
				{
					stream >> buffer;
					sscanf_s(buffer.c_str(), "%d/%d", &f[ii], &ft[ii]);

					DEBUG_PRINT(std::cout << "(" << f[ii] << ", " << ft[ii] << ")" << std::endl);
				}

				//mesh->face.push_back(f);
				//mesh->faceTex.push_back(ft);
			}
			break;
			case 2:
			{
				if (hasDoubleSlash)
				{ // caras y normales

					for (int ii = 0; ii < 3; ii++)
					{
						stream >> buffer;
						sscanf_s(buffer.c_str(), "%d//%d", &f[ii], &fn[ii]);
						DEBUG_PRINT(std::cout << "(" << f[ii] << ", " << fn[ii] << ")" << std::endl);
					}

					//mesh->face.push_back(f);
					//mesh->faceNorm.push_back(fn);

				}
				else
				{ //caras texturas y normales
					for (int ii = 0; ii < 3; ii++)
					{
						stream >> buffer;
						sscanf_s(buffer.c_str(), "%d/%d/%d", &f[ii], &ft[ii], &fn[ii]);
						DEBUG_PRINT(std::cout << "(" << f[ii] << ", " << ft[ii] << ", " << fn[ii] << ")" << std::endl);
					}

					//mesh->face.push_back(f);
					//mesh->faceNorm.push_back(fn);
					//mesh->faceTex.push_back(ft);
				}
			}

			break;
			}
		}
		else if (tofData == "usemtl")
		{
			stream >> buffer;// pongo el nombre del material
							 //mesh->materials.push_back(materials[buffer]);
		}
	}
	return file;
}

class Material;
class ShaderProgram;
std::istream& operator>> (std::istream& file, Material& material);

class Material : public Asset {
public:
	ShaderProgram * shader = nullptr;


	void Bind() {
		//shader.setFloat("shininess", shiny);
		//////shader.setFloat("roughness", mat.rough);
		////shader.setFloat("covariance", mat.covariance);
		////shader.setFloat("reflectance", mat.reflectance);
		//shader.setVec3("kD", kD);
		//shader.setVec3("kS", kS);
		//shader.setVec3("kE", kE);
		//shader.setVec3("kA", kA);
	}


	std::map<std::string, Texture*> maps;
	Material(std::string n) : Asset(n) {

	}
	Vec3 kA;
	Vec3 kD;
	Vec3 kS;
	Vec3 kE{ 0,0,0 };
	float shiny = 0;
	float refractionIndex = 1;
	float dissolve = 1;

	//http://paulbourke.net/dataformats/mtl/
	static std::vector<Material*> ReadMTLLIB(std::string matPath) {

		std::vector<Material*> materials;
		std::ifstream file(matPath, std::ios::in);

		if (!file.is_open())
		{
			DEBUG_PRINT(std::cout << "Couldn't open material file" << matPath << std::endl);
			__debugbreak();

			return materials;
		}

		std::string buffer;
		while (!file.eof() && file.good())
		{
			std::string tofData;
			std::getline(file, buffer);

			std::stringstream stream(buffer);

			stream >> tofData;
			DEBUG_PRINT(std::cout << buffer << std::endl);
			if (tofData == "newmtl") {
				std::cout << buffer << std::endl;
				stream >> tofData;
				Material *mat(new Material(tofData));
				file >> *mat;
				materials.push_back(mat);
			}
		}
		DEBUG_PRINT(std::cout << "--------------------------" << std::endl);
		DEBUG_PRINT(std::cout << "--------------------------" << std::endl);

		return materials;
	}

};

std::istream& operator>> (std::istream& file, Material& material)
{
	std::string buffer;
	while (!file.eof() && file.good())
	{
		std::string tofData;

		if (file.peek() == 'n') {
			return file;
		}

		std::getline(file, buffer);

		std::stringstream stream(buffer);

		stream >> tofData;
		std::cout << buffer << std::endl;
		{


			// para los K falta espectral y xyz
			if (tofData == "Ka") {
				stream >> material.kA;
			}
			else if (tofData == "Kd") {
				stream >> material.kD;
			}
			else if (tofData == "Ks") {
				stream >> material.kS;
			}
			else if (tofData == "Ns") {
				stream >> material.shiny;
			}
			else if (tofData == "Ni") {
				stream >> material.refractionIndex;
			}
			else if (tofData == "d") {
				stream >> material.dissolve;
			}
			else if (tofData == "Tf") {

			}
			else if (tofData == "illum") {
				int type;
				stream >> type;
				material.shader = ShaderProgram::GetDefault(static_cast<IllumModel>(type));
			}
			else if (tofData == "map_Ka") {

				stream >> tofData;

				if (material.maps.find(tofData) == material.maps.end())
				{
					material.maps[tofData] = new Texture(tofData, MapType::AMBIENT);
				}

			}
			else if (tofData == "map_Kd") {

				stream >> tofData;

				if (material.maps.find(tofData) == material.maps.end())
				{
					material.maps[tofData] = new Texture(tofData, MapType::DIFFUSE);

				}

			}
			else if (tofData == "map_Ks") {


			}
			else if (tofData == "map_Ns") {
				stream >> tofData;

				if (material.maps.find(tofData) == material.maps.end())
				{
					material.maps[tofData] = new Texture(tofData, MapType::SHINY);

				}


			}
			else if (tofData == "map_d") {

			}
			else if (tofData == "disp") {
				stream >> tofData;

				if (material.maps.find(tofData) == material.maps.end())
				{
					material.maps[tofData] = new Texture(tofData, MapType::DISPLACEMENT);

				}

			}
			else if (tofData == "decal") {


			}
			else if (tofData == "bump") {

				stream >> tofData;

				if (material.maps.find(tofData) == material.maps.end())
				{
					material.maps[tofData] = new Texture(tofData, MapType::BUMP);

				}
			}
		}
	}

	return file;
}

//http://paulbourke.net/dataformats/obj/
class Mesh : public Asset {
public:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	std::vector<Vertex> vertex;
	std::vector<Material*> materials;
	std::vector<SubMesh*> submesh;

	SubMesh& AddSubMesh(std::string n) {
		SubMesh * m = new SubMesh(*this, n);
		submesh.push_back(m);
		return *m;
	}

	Mesh(const std::string path)
		: Asset(path)
	{
		std::ifstream file(path, std::ios::in);
		std::string buffer;
		if (!file.is_open())
		{
			DEBUG_PRINT(std::cout << "Couldn't open file" << path << std::endl);
			__debugbreak();
			std::cin.ignore();
			exit(-1);
		}


		while (!file.eof() && file.good())
		{
			std::getline(file, buffer);
			std::stringstream stream(buffer);
			DEBUG_PRINT(std::cout << buffer << std::endl);
			{
				std::string tofData;
				stream >> tofData;

				if (tofData[0] == '#')
				{
					continue;
				}

				if (tofData == "o") {
					stream >> tofData;
					SubMesh &m = AddSubMesh(tofData);
					file >> m;
				}
				else if (tofData == "mtllib") {
					std::cout << buffer << std::endl;

					stream >> tofData;
					std::string matPath = path.substr(0, path.find_last_of("/"));
					matPath.append("/" + tofData);

					materials = Material::ReadMTLLIB(matPath);

				}
				else if (tofData == "s")
				{
					stream >> buffer;

					// if
				}
			}
			DEBUG_PRINT(std::cout << "--------------------------" << std::endl);
			DEBUG_PRINT(std::cout << "--------------------------" << std::endl);
		}

		////for (auto mesh : meshes)
		////{

		////	mesh->face.pop_back();
		////}

		////for (auto mesh : meshes)
		////{

		////	size_t numVertex = mesh->face.size();

		////	std::vector<std::vector<iVec3>*> arrs({ &mesh->face, &mesh->faceNorm, &mesh->faceTex });
		////	std::vector<std::vector<Vec3>*> arrsTarg({ &mesh->vertex, &mesh->vertexNormal, &mesh->uv });
		////	std::vector<Vec3> aux;

		////	for (unsigned int ii = 0; ii < arrs.size(); ii++) {
		////		auto &arr = *arrs[ii];
		////		auto &arrTarget = *arrsTarg[ii];

		////		for (auto &f : arr) {
		////			//std::cout << f.x << " " << f.y << " " << f.z << std::endl;

		////			for (int ii = 0; ii < 3; ii++) {
		////				glm::u32& index = f[ii];
		////				if (index > 0) {
		////					index--;
		////				}
		////				else {
		////					index = (unsigned int)(numVertex + index);
		////				}


		////			}
		////			aux.push_back(arrTarget[f[0]]);
		////			aux.push_back(arrTarget[f[1]]);
		////			aux.push_back(arrTarget[f[2]]);

		////		}
		////		arrTarget = std::move(aux);


		////	}


		////}


	}

	void GLCreate() {

		glBindVertexArray(VAO);
		// 2. copy our vertices array in a vertex buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertex.size(), vertex.data(), GL_STATIC_DRAW);
		// 3. copy our index array in a element buffer for OpenGL to use
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// 4. then set the vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

	}

};
#pragma endregion Models

class FrameBuffer {

	//For a framebuffer to be complete the following requirements have to be satisfied :

	// We have to attach at least one buffer(color, depth or stencil buffer).
	// There should be at least one color attachment.
	// All attachments should be complete as well(reserved memory).
	// Each buffer should have the same number of samples.
	unsigned int fbo;

public:

	FrameBuffer() {
		GLCALL(glGenFramebuffers(1, &fbo));



		if (!glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
			__debugbreak();
		}
	}

	~FrameBuffer() {
		GLCALL(glDeleteFramebuffers(1, &fbo));
	}
	void AttachTexture(const Texture &tex, FBAttachment attchType) {
		Bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<int>(attchType), GL_TEXTURE_2D, tex.id, 0);
		UnBind();

	}

	void Bind() {
		// By binding to the GL_FRAMEBUFFER target all the next read and write 
		// framebuffer operations will affect the currently bound framebuffer.
		// It is also possible to bind a framebuffer to a read or write target 
		// specifically by binding to GL_READ_FRAMEBUFFER or GL_DRAW_FRAMEBUFFER respectively.
		// The framebuffer bound to GL_READ_FRAMEBUFFER is then used for all read operations like 
		// glReadPixels and the framebuffer bound to GL_DRAW_FRAMEBUFFER is used as the destination for rendering,
		// clearing and other write operations.Most of the times you won't need to make this distinction though and 
		// you generally bind to both with GL_FRAMEBUFFER.


		GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
	}

	void UnBind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};





SystemRenderer *sRenderer;






