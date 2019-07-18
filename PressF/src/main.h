#pragma once

#pragma region Includes

#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_opengl.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>


#define PF_INFO(...)      //spdlog::info(__VA_ARGS__)
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
#include <iostream>
#include <sstream>
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


//The illumination models are :
//
//0  This is a constant color illumination model.The color is the
//specified Kd for the material.The formula is :
//
//color = Kd
//
//1  This is a diffuse illumination model using Lambertian shading.The
//color includes an ambient constant term and a diffuse shading term for
//each light source.The formula is
//
//color = KaIa + Kd{ SUM j = 1..ls, (N * Lj)Ij }
//
//2  This is a diffuse and specular illumination model using Lambertian
//shading and Blinn's interpretation of Phong's specular illumination
//model(BLIN77).The color includes an ambient constant term, and a
//diffuse and specular shading term for each light source.The formula
//is :
//
//color = KaIa
//+ Kd{ SUM j = 1..ls, (N*Lj)Ij }
//+Ks{ SUM j = 1..ls, ((H*Hj) ^ Ns)Ij }
//
//3  This is a diffuse and specular illumination model with reflection
//using Lambertian shading, Blinn's interpretation of Phong's specular
//illumination model(BLIN77), and a reflection term similar to that in
//Whitted's illumination model (WHIT80).  The color includes an ambient 
//constant term and a diffuse and specular shading term for each light
//source.The formula is :
//
//color = KaIa
//+ Kd{ SUM j = 1..ls, (N*Lj)Ij }
//+Ks({ SUM j = 1..ls, ((H*Hj) ^ Ns)Ij } +Ir)
//
//Ir = (intensity of reflection map) + (ray trace)
//
//4  The diffuse and specular illumination model used to simulate glass
//is the same as illumination model 3.  When using a very low dissolve
//(approximately 0.1), specular highlights from lights or reflections
//become imperceptible.
//
//Simulating glass requires an almost transparent object that still
//reflects strong highlights.The maximum of the average intensity of
//highlights and reflected lights is used to adjust the dissolve factor.
//The formula is :
//
//color = KaIa
//+ Kd{ SUM j = 1..ls, (N*Lj)Ij }
//+Ks({ SUM j = 1..ls, ((H*Hj) ^ Ns)Ij } +Ir)
//
//5  This is a diffuse and specular shading models similar to
//illumination model 3, except that reflection due to Fresnel effects is
//introduced into the equation.Fresnel reflection results from light
//striking a diffuse surface at a grazing or glancing angle.When light
//reflects at a grazing angle, the Ks value approaches 1.0 for all color
//samples.The formula is :
//
//color = KaIa
//+ Kd{ SUM j = 1..ls, (N*Lj)Ij }
//+Ks({ SUM j = 1..ls, ((H*Hj) ^ Ns)Ij Fr(Lj*Hj,Ks,Ns)Ij } +
//	Fr(N*V, Ks, Ns)Ir})
//
//
//	6  This is a diffuse and specular illumination model similar to that
//	used by Whitted(WHIT80) that allows rays to refract through a surface.
//	The amount of refraction is based on optical density(Ni).The
//	intensity of light that refracts is equal to 1.0 minus the value of Ks,
//	and the resulting light is filtered by Tf(transmission filter) as it
//	passes through the object.The formula is :
//
//color = KaIa
//+ Kd{ SUM j = 1..ls, (N*Lj)Ij }
//+Ks({ SUM j = 1..ls, ((H*Hj) ^ Ns)Ij } +Ir)
//+ (1.0 - Ks) TfIt
//
//7  This illumination model is similar to illumination model 6, except
//that reflection and transmission due to Fresnel effects has been
//introduced to the equation.At grazing angles, more light is reflected
//and less light is refracted through the object.The formula is :
//
//color = KaIa
//+ Kd{ SUM j = 1..ls, (N*Lj)Ij }
//+Ks({ SUM j = 1..ls, ((H*Hj) ^ Ns)Ij Fr(Lj*Hj,Ks,Ns)Ij } +
//	Fr(N*V, Ks, Ns)Ir})
//
//	+ (1.0 - Kx)Ft(N*V, (1.0 - Ks), Ns)TfIt
//
//	8  This illumination model is similar to illumination model 3 without
//	ray tracing.The formula is :
//
//color = KaIa
//+ Kd{ SUM j = 1..ls, (N*Lj)Ij }
//+Ks({ SUM j = 1..ls, ((H*Hj) ^ Ns)Ij } +Ir)
//
//Ir = (intensity of reflection map)
//
//9  This illumination model is similar to illumination model 4without
//ray tracing.The formula is :
//
//
//color = KaIa
//+ Kd{ SUM j = 1..ls, (N*Lj)Ij }
//+Ks({ SUM j = 1..ls, ((H*Hj) ^ Ns)Ij } +Ir)
//
//Ir = (intensity of reflection map)
//
//10  This illumination model is used to cast shadows onto an invisible
//surface.This is most useful when compositing computer - generated
//imagery onto live action, since it allows shadows from rendered objects
//to be composited directly on top of video - grabbed images.The equation
//for computation of a shadowmatte is formulated as follows.
//
//color = Pixel color.The pixel color of a shadowmatte material is
//always black.
//
//color = black
//
//M = Matte channel value.This is the image channel which typically
//represents the opacity of the point on the surface.To store the shadow
//in the matte channel of the image, it is calculated as :
//
//M = 1 - W / P
//
//where :
//
//	P = Unweighted sum.This is the sum of all S values for each light :
//
//	P = S1 + S2 + S3 + .....
//
//	W = Weighted sum.This is the sum of all S values, each weighted by
//	the visibility factor(Q) for the light :
//
//W = (S1 * Q1) + (S2 * Q2) + .....
//
//Q = Visibility factor.This is the amount of light from a particular
//light source that reaches the point to be shaded, after traveling
//through all shadow objects between the light and the point on the
//surface.Q = 0 means no light reached the point to be shaded; it was
//blocked by shadow objects, thus casting a shadow.Q = 1 means that
//nothing blocked the light, and no shadow was cast.  0 < Q < 1 means that
//	the light was partially blocked by objects that were partially
//	dissolved.
//
//	S = Summed brightness.This is the sum of the spectral sample
//	intensities for a particular light.The samples are variable, but the
//default is 3:
//
//S = samp1 + samp2 + samp3.

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

#include "Loaders.h"

#pragma region Shading

class Shader : public Asset {
	std::string src;
	unsigned int id = 0;
	int type;
	Shader(): Asset("Shader"){}
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


	//ShaderProgram GetDefault(IllumModel model) {
	//}



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
};

class Transform {
public:
	Transform(GameObject& go) :
		gameobject(go)
	{

	}
	GameObject &gameobject;
	Vec3 position{ 0, 0, 0 };
	Vec3 rotation{ 0, 0, 0 };
	Vec3 scale{ 1, 1, 1 };
	Mat4 model;
	bool isDirty = true;

	Mat4& GetModel() {
		model = Mat4(1);
		model = glm::rotate(model, rotation.x, { 1,0,0 });
		model = glm::rotate(model, rotation.y, { 0,1,0 });
		model = glm::rotate(model, rotation.z, { 0,0,1 });
		model = glm::translate(model, position);
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
			if (comp->enabled) {
				comp->Update();
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




#pragma region Components
class Camera : public Component {

	friend class GameObject;
	float fov = 45;
	unsigned int power = 10000;
	Camera(COMP_PARAMS) INIT_COMP("Camera")
	{

	}

	// Inherited via Component
	virtual void Update() override {
		PF_INFO("Camera running");
	}


	// Inherited via Component
	virtual void UI() override {
		fov = nk_propertyf(ctx, "FOV", 30.f, fov, 120.0f, 0.01f, 0.005f);
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
	Mesh & mesh;
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
		PF_INFO("PointLight running");
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
		PF_INFO("SpotLight running");

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
		PF_INFO("DirectionalLight running");


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

class SystemRenderer {
public:
	std::list<Light*> lights;
	std::list<Camera*> camera;
	std::list<Renderer*> renderers;
	CubeMap cubemap;
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
			ren->Render();
		}



		cubemap.Render();
	}
};

SystemRenderer *sRenderer;






