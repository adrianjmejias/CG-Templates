#pragma once

#pragma region Includes
#include <OBJ_Loader.h>
#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_opengl.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <glm/gtc/random.hpp>

#define PF_INFO(...)      spdlog::info(__VA_ARGS__)
#define PF_ERROR(...)     spdlog::error(__VA_ARGS__)
#define PF_DEBUG(...)     spdlog::debug(__VA_ARGS__)
#define PF_WARN(...)      spdlog::warn(__VA_ARGS__)
#define PF_CRITICAL(...)  spdlog::critical(__VA_ARGS__)
#define forn(var,init,end) for(int var = init; var<end; var++)
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
int UNIVERSALID = 0;

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <iostream>
#include <numeric>
#include <sstream>
#include <fstream>
#include <initializer_list>
#include <stddef.h>
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
#define STL_BYTE_SIZE(cont, type) (cont.size() * sizeof(type))

struct nk_context *ctx;
struct nk_colorf bg;

template <typename TT>
class Delta {
	TT last;
	TT delta;
public:
	Delta(const TT &init) {
		last = init;
	}
	TT GetValue() {
		return delta;
	}
	TT Update(const TT &newVal) {
		delta = newVal - last;
		last = newVal;
	}
};

Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;
double deltaTime = 0;

using MousePosType = int;
GLint GO_ID = 0;
MousePosType mouse_lastPosX = 0;
MousePosType mouse_lastPosY = 0;
MousePosType mouse_deltaX = 0;
MousePosType mouse_deltaY = 0;

#pragma region Types

using Color = nk_colorf;
using Vec3 = glm::vec3;
using Vec2 = glm::vec2;
using Vec4 = glm::vec4;
using iVec3 = glm::ivec3;

#define NKE_COLOR2VEC4(c) (Vec4{c.r,c.g,c.b,c.a})

std::string to_string(const Vec3& v) {
	char c[30];

	std::string s = "(";

	sprintf(c, "%.3f", v[0]);

	s.append(c);
	s.append(", ");

	sprintf(c, "%.3f", v[1]);

	s.append(c);
	s.append(", ");

	sprintf(c, "%.3f", v[2]);

	s.append(c);
	s.append(" )");

	return s;
}

Vec3 nke_Vec3(Vec3 v, const std::string &label, const float min, const float max) {
	//sprintf(buffer, "%.2f, %.2f, %.2f", position[0], position[1], position[2]);
	if (nk_combo_begin_label(ctx, (label + to_string(v)).data(), nk_vec2(200, 200))) {
		nk_layout_row_dynamic(ctx, 25, 1);

		v.x = nk_propertyf(ctx, "X:", min, v.x, max, 1, 0.01f);
		v.y = nk_propertyf(ctx, "Y:", min, v.y, max, 1, 0.01f);
		v.z = nk_propertyf(ctx, "Z:", min, v.z, max, 1, 0.01f);

		nk_combo_end(ctx);
	}

	return v;
}

void nke_Color(Color &c, std::string label) {
	nk_label(ctx, label.c_str(), nk_text_align::NK_TEXT_ALIGN_LEFT);
	if (nk_combo_begin_color(ctx, nk_rgb_cf(c), nk_vec2(200, 400))) {
		nk_layout_row_dynamic(ctx, 120, 1);
		c = nk_color_picker(ctx, c, NK_RGBA);
		nk_layout_row_dynamic(ctx, 25, 2);
		nk_layout_row_dynamic(ctx, 25, 1);

		c.r = nk_propertyf(ctx, "#R:", 0, c.r, 1.0f, 0.01f, 0.005f);
		c.g = nk_propertyf(ctx, "#G:", 0, c.g, 1.0f, 0.01f, 0.005f);
		c.b = nk_propertyf(ctx, "#B:", 0, c.b, 1.0f, 0.01f, 0.005f);
		c.a = nk_propertyf(ctx, "#A:", 0, c.a, 1.0f, 0.01f, 0.005f);
		nk_combo_end(ctx);
	}
}

std::ostream& operator<<(std::ostream& os, const Vec3& v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}
std::istream& operator>> (std::istream& is, Vec3& v)
{
	is >> v.x >> v.y >> v.z;
	return is;
}
std::istream& operator>> (std::istream& is, Vec4& v)
{
	is >> v.x >> v.y >> v.z >> v.w;
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
	BUMP,
	CUBEMAP,
	SHINY,
	DISPLACEMENT,
	DECAL,
	REFLECTION,
	DISSOLVE,
};

// Defines several possible options for camera movement.Used as abstraction to stay away from window - system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class CameraGL
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Constructor with vectors
	CameraGL(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, 1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	// Constructor with scalar values
	CameraGL(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
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

#pragma endregion Types

template <typename TT>
struct VectorGl : public std::vector<TT> {
	unsigned int id;
	unsigned int type;
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
class SubMesh;
class SystemRenderer;
class ShaderProgram;
class RenderSpec;
class Light;
class MeshRenderer;
class CubeMap;

class Material;
using EmptyFunction = std::function<void()>;
using PreReqFunc = std::function<void(EmptyFunction)>;
#define HEADER_LAMBDA [&](EmptyFunction draw)





class Asset {
public:

	Asset(const std::string& n) : name(n)
	{
	}
	Asset(const std::string& n, const std::string& p) : name(n), path(p)
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
	Shader(std::string n) : Asset(n) {}
public:
	unsigned int Get() { return id; }

	static Shader* FromString(const std::string str, unsigned int type) {
		auto s = new Shader(str);
		s->SetFromString(str, type);
		return s;
	}
	static Shader* FromPath(const std::string path, unsigned int type) {
		auto s = new Shader(path);
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

		__debugbreak();
		throw std::exception("Not implemented");
	}
};

ShaderProgram *shaders[30];
class ShaderProgram :
	protected std::vector< Shader *>
{
private:
	unsigned int id;
public:
	bool lit = true;
	bool viewDependant = true;
	bool MVP = true;
	bool shadows = true;
	PreReqFunc preReq;
	static ShaderProgram* GetDefault(IllumModel model) {
		ShaderProgram *a = shaders[static_cast<int>(model)];

		if (a) {
			return a;
		}

		else {
			PF_WARN("DEFUALT SHADER USED");
			return shaders[static_cast<int>(IllumModel::COOK)];
		}


		__debugbreak();
		throw std::exception("NOT RECOGNIZED SHADER");
	}

	ShaderProgram(std::vector<Shader* > li, const PreReqFunc &f)
		: std::vector<Shader* >(std::move(li)), preReq(f)
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

#pragma region shaderDcl
	/**
	* Enables the shader to be use
	*/
	void  use() const;

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
#pragma endregion shaderDcl

};

void Shader::ReCompile() {
	PF_INFO("Recompiling {0}", name);
	SetFromFile(path, type);
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
		__debugbreak();
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

void ShaderProgram::Compile() {
	GLCALL(id = glCreateProgram());
	//std::cout << "Loading Program with --------------------------------------------------------" << std::endl;

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

		PF_ERROR("Failed to compile program {0} :(", at(0)->name);
		PF_ERROR(message);

		__debugbreak();
	}

	//std::cout << "Program loaded --------------------------------------------------------" << std::endl;
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

void ShaderProgram::use() const
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
std::vector<Mesh*> meshes;

class Component {
	friend class GameObject;
protected:
	int enabled = true;
public:
	std::string name;
	int openUI = false;
	Transform &transform;
	GameObject &gameobject;


	bool Enabled();
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
	Vec3 right = Transform::WorldRight();
	Vec3 up = Transform::WorldUp();
	Vec3 front = Transform::WorldFront();

	Vec3 position{ 0, 0, 0 };
	Vec3 rotation{ 0, 0, 0 };
	Vec3 scale{ 1, 1, 1 };
	Mat4 model = Mat4(1);
	Mat4 acum = Mat4(1);
	Mat4 rotMat = Mat4(1);

	Dirty dirty = Dirty::None;

#pragma region mutators

	Vec3 GetRotation() { return rotation; }
	Vec3 GetScale() { return scale; }
	Vec3 GetPosition() { return position; }
	Transform* SetRotation(const Vec3& val) {
		SetDirty(Dirty::Model);
		rotation = val;

		//forn(ii, 0, 3){
		//	if (rotation[ii] > 360 || rotation[ii] < -360) {
		//		rotation[ii] = (glm::abs(rotation[ii]) - 360)  * glm::sign(rotation[ii]);
		//	}
		//}



		return this;
	}
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
	Vec3 Front() { return front; }
	Vec3 Right() { return right; }
	Vec3 Up() { return up; }

	static Vec3 WorldFront() {
		return Vec3{ 0,0,1 };
	}
	static Vec3 WorldRight() {
		return Vec3{ 1,0,0 };
	}
	static Vec3 WorldUp() {
		return Vec3{ 0,1,0 };
	}
	static Vec3 RotatePoint(Vec3 point, Vec3 rotation) {
		return  RotatePoint(point, GenRotMat(rotation));
	}
	static Vec3 RotatePoint(Vec3 point, Mat4 rotation) {
		return  rotation * Vec4(point, 1);
	}

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
public:
	bool TryGetClean();

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
		return Transform::GenModel(scale, position, GenRotMat(rotation));
	}
	static Mat4 GenModel(const Vec3 &scale, const Vec3 &position, const Mat4 &rotation) {
		Mat4 model = Mat4(1);
		model = glm::scale(model, scale);
		model = glm::translate(model, position);
		model = rotation * model;
		return model;
	}

	static Mat4 GenRotMat(const Vec3 &rotation) {
		Mat4 rot = glm::rotate(Mat4(1), glm::radians(rotation.x), Vec3(1, 0, 0));
		rot = glm::rotate(rot, glm::radians(rotation.y), Vec3(0, 1, 0));
		rot = glm::rotate(rot, glm::radians(rotation.z), Vec3(0, 0, 1));
		return rot;
	}
};


class GameObject {
	int _id;
	friend class SystemRenderer;
	std::list<Component*> components;
	int enabled = true;
public:


	// esto puede llegar a ser orden N y debe ser seteado como un evento asi como lo es Dirty
	bool IsEnabled() {
		return enabled;// && (transform.parent) ? transform.parent->gameobject.IsEnabled() : true;
	}
	int openUI = false;
	Transform transform;
	std::string name;
	GameObject(const std::string &n) : transform(*this) {
		_id = GO_ID++;
		name = std::to_string(_id).append(n);
	}
	void Update() {
		if (IsEnabled())
			for each (auto comp in components)
			{
				PF_ASSERT(comp && "COMPONENT IS NULL");

				//PF_INFO("Object {0}", name);
				if (comp->Enabled()) {
					transform.TryGetClean();
					comp->Update();
				}
			}
	}

	void HandleEvent(const SDL_Event &e) {
		for each (auto comp in components)
		{
			PF_ASSERT(comp && "COMPONENT IS NULL");
			if (comp->Enabled()) {
				//PF_INFO("HANDLE EVENT ,{0}", this->name);
				comp->HandleEvent(e);
			}
		}
	}
	void UI() {
		if (nk_tree_push_hashed(ctx, NK_TREE_TAB, name.data(), static_cast<nk_collapse_states>(openUI), name.data(), 5, __LINE__)) {
			// height, width, row
			const int val = 6000;
			//nk_layout_row_static(ctx, 18, 100, 1);
			nk_checkbox_label(ctx, "Enabled", &enabled);
			transform.SetPosition(nke_Vec3(transform.GetPosition(), "Position", -val, val));
			transform.SetRotation(nke_Vec3(transform.GetRotation(), "Rotation", -val, val));
			transform.SetScale(nke_Vec3(transform.GetScale(), "Scale", -val, val));
			for each (auto comp in components)
			{
				PF_ASSERT(comp && "component is null");

				if (nk_tree_push_hashed(ctx, NK_TREE_TAB, comp->name.data(), static_cast<nk_collapse_states>(comp->openUI), "hashin\0", 7, __LINE__))
				{
					nk_checkbox_label(ctx, "Enabled", &comp->enabled);

					comp->UI();

					nk_tree_pop(ctx);
				}
			}

			if (transform.children.size() > 0) {
				nk_label(ctx, "Children", nk_text_align::NK_TEXT_ALIGN_LEFT);
			}

			for each (Transform* child in transform.children)
			{
				child->gameobject.UI();
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
	template <typename TT>
	TT& GetComponent() {
		for each (object comp in component)
		{
			if (TT* t = dynamic_cast<TT*>(comp)) {
				return t;
			}
		}
		return nullptr;
	}
};

bool Transform::TryGetClean() {
	if (dirty == Dirty::None) return false;
	//PF_INFO("Cleaned {0}/ pos/ rot/scale, front, right,up", gameobject.name);
	//std::cout << position << " " << rotation << " " << scale << std::endl;
	//std::cout << front << " " << right << " " << up << std::endl;
	if (dirty == Dirty::Model) {
		rotMat = Transform::GenRotMat(rotation);

		up = RotatePoint(WorldUp(), rotMat);
		right = RotatePoint(WorldRight(), rotMat);
		front = RotatePoint(WorldFront(), rotMat);

		if (parent) {
			const Mat4& parentRot = parent->rotMat;
			up = RotatePoint(up, parentRot);
			right = RotatePoint(right, parentRot);
			front = RotatePoint(front, parentRot);
		}

		up = glm::normalize(up);
		right = glm::normalize(right);
		front = glm::normalize(front);

		model = Transform::GenModel(scale, position, rotMat);
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
			PF_ASSERT(child && "you shouldn't have a null child");
			child->SetDirty(Dirty::Acum);
		}
	}

	dirty = Dirty::None;
	return true;
}

#pragma region Components
class Camera : public Component {

	friend class GameObject;
public:
	int power = 10000;
	int isPerspective = true;
	float speed = 25.f;
	float sensitivity = 0.3f;
	float fov = 45;
	float nearClippingPlane = 0.1f;
	float farClippingPlane = 200.0f;

	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;

	float Yaw;
	float Pitch;

	Mat4 projection;
	Mat4 view;

	Camera(COMP_PARAMS) INIT_COMP("Camera")
	{
		Pitch = -48.05f;
		Yaw = 12.88f;
		//transform.SetPosition({9.42f, -1.24, 5.14});
		sensitivity = 0.10f;
		fov = 70;
		updateCameraVectors();
	}
public:
	// Inherited via Component
	virtual void Update() override {
	}

	// Inherited via Component
	virtual void UI() override {
		nk_checkbox_label(ctx, "IsPerspective", &isPerspective);
		power = nk_propertyi(ctx, "Power", 0, power, 10000, 1.f, 1.f);
		fov = nk_propertyf(ctx, "Fov", 30.f, fov, 120.0f, 0.01f, 0.005f);
		nearClippingPlane = nk_propertyf(ctx, "Near", 0.001, nearClippingPlane, farClippingPlane, 1.f, 0.005f);
		farClippingPlane = nk_propertyf(ctx, "Far", nearClippingPlane, farClippingPlane, 6000, 1.f, 0.005f);
		speed = nk_propertyf(ctx, "Speed", 50, speed, 500, 1.f, 0.005f);
		sensitivity = nk_propertyf(ctx, "Sensitivity", 0.001f, sensitivity, 3, 00.1f, 0.005f);
	}

	virtual void HandleEvent(const SDL_Event &e) override {
		if (!SDL_GetRelativeMouseMode()) return;
		if (e.type == SDL_EventType::SDL_KEYDOWN) {
			//if (e.key.keysym.mod == SDL_Scancode::SDL_SCANCODE_LSHIFT)
			{
				float scaledSpeed = static_cast<float>(deltaTime) * speed;
				if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_W) {
					transform.Translate(Front * scaledSpeed);
				}
				if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_S) {
					transform.Translate(-Front * scaledSpeed);
				}
				if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_A) {
					transform.Translate(-Right * scaledSpeed);
				}
				if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_D) {
					transform.Translate(Right* scaledSpeed);
				}
			}
		}

		if (e.type == SDL_EventType::SDL_MOUSEMOTION) {
			float movX = sensitivity * e.motion.xrel * deltaTime;
			float movY = sensitivity * e.motion.yrel* deltaTime;

			float xoffset = e.motion.xrel* sensitivity;
			float yoffset = e.motion.yrel*sensitivity;

			Yaw += xoffset;
			Pitch += yoffset;

			// Make sure that when pitch is out of bounds, screen doesn't get flipped
			//if (constrainPitch)
			{
				if (Pitch > 89.0f)
					Pitch = 89.0f;
				if (Pitch < -89.0f)
					Pitch = -89.0f;
			}

			transform.SetRotation(glm::degrees(Front));
			// Update Front, Right and Up Vectors using the updated Euler angles
			updateCameraVectors();
		}
	}

	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, Transform::WorldUp()));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
	virtual Mat4& GetView() {
		view = glm::lookAt(transform.GetPosition(), transform.GetPosition() + Front, Up);
		//view = transform.GetAccumulated();
		//view = Transform::GenModel(-transform.GetPosition(), -transform.GetRotation(), transform.GetScale());
		//return view;
		return view;
	}
	virtual Mat4& GetProjection() {
		int w, h;
		SDL_GetWindowSize(win, &w, &h);

		if (isPerspective)
		{
			projection = glm::perspective(glm::radians(fov), static_cast<float>(w) / h, nearClippingPlane, farClippingPlane);
		}
		else {
			projection = glm::ortho(0.f, static_cast<float>(w), 0.f, static_cast<float>(h), nearClippingPlane, farClippingPlane);
		}

		return projection;
	}
};

#pragma region Light
enum class LightType {
	POINT,
	DIRECTIONAL,
	SPOTLIGHT
};

#define RENDERER_PARAMS COMP_PARAMS

class Light : public Component {
public:

	LightType type;
	Color kA{ 1,1,1,1 };
	Color kD{ 0,0,1,1 };
	Color kS{ 1,0,0,1 };
	Color kE{ 0,0,0,1 };
	Vec3 attenuation;
	float innerAngle = 15.f;
	float outterAngle = 20.f;

	Light(COMP_PARAMS, LightType lt) : Component(o, t, "Light"), type(lt) {}

	void Bind(iVec3& countLights, const ShaderProgram &shader) {
#define SEND_DATA_JOINED
		int& myCount = countLights[static_cast<int>(type)];

#ifdef SEND_DATA_SEPARATED
		switch (type)
		{
		case LightType::DIRECTIONAL:
			name = "directionals[" + std::to_string(myCount) + "]";
			break;
		case LightType::POINT:
			name = "points[" + std::to_string(myCount) + "]";
			shader.setVec3(name + ".position", transform.GetPosition());
			shader.setVec3(name + ".attenuation", attenuation);
			break;
		case LightType::SPOTLIGHT:
			name = "spots[" + std::to_string(myCount) + "]";
			shader.setVec3(name + ".position", transform.GetPosition());
			shader.setVec3(name + ".attenuation", attenuation);
			shader.setFloat(name + ".innerAngle", glm::radians(innerAngle));
			shader.setFloat(name + ".outerAngle", glm::radians(outterAngle));
			break;
		default:
			__debugbreak(); // this should never happen
			break;
		}
#endif // SEND_DATA_SEPARATED

#ifdef SEND_DATA_JOINED
		name = "LIGHTS[" + std::to_string(myCount) + "]";

		shader.setInt(name + ".type", static_cast<int>(type));
		shader.setVec3(name + ".position", transform.GetPosition());
		shader.setVec3(name + ".attenuation", attenuation);
		shader.setFloat(name + ".innerAngle", glm::radians(innerAngle));
		shader.setFloat(name + ".outerAngle", glm::radians(outterAngle));
#endif // SEND_DATA_JOINED

		shader.setVec3(name + ".direction", transform.front);
		shader.setBool(name + ".isOn", enabled);

		shader.setVec4(name + ".kA", NKE_COLOR2VEC4(kA));
		shader.setVec4(name + ".kD", NKE_COLOR2VEC4(kD));
		shader.setVec4(name + ".kS", NKE_COLOR2VEC4(kS));

		myCount++;
	}
	void Update() override {
	}

	void UI() override {
		switch (type)
		{
		case LightType::POINT:
			break;
		case LightType::DIRECTIONAL:
			break;
		case LightType::SPOTLIGHT:
			attenuation = nke_Vec3(attenuation, "Attenuation", 0, 20);
			innerAngle = nk_propertyf(ctx, "innerAngle", 0, innerAngle, outterAngle, 0.01f, 0.005f);
			outterAngle = nk_propertyf(ctx, "outterAngle", innerAngle, outterAngle, 180, 0.01f, 0.005f);
			break;
		default:
			break;
		}
		nke_Color(kA, "kA");
		nke_Color(kD, "kD");
		nke_Color(kS, "kS");
		nke_Color(kE, "kE");
	}
	void HandleEvent(const SDL_Event &e) override {}
	void ShadowPass() {}
};

#pragma endregion  Components


class MeshRenderer : public Component {
public:
	Mesh & mesh;
	MeshRenderer(COMP_PARAMS, Mesh &m);
	virtual void Update() {};
	virtual void UI() {};
	void HandleEvent(const SDL_Event &e) override;
};

class Texture {
public:
	unsigned int id;
	int width;
	int height;
	int nChannels;
	ClampType clamp = ClampType::REPEAT;
	TexInterpolation texInterpolation = TexInterpolation::LINEAR;
	MapType type = MapType::AMBIENT;
	float borderColor[4] = { 1,1,1,1 };

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
		//PF_INFO("texture loaded {0}", path);
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
		tex = new Texture(faces);
		shader = ShaderProgram::GetDefault(IllumModel::CUBEMAP);

		// skybox VAO
		GLCALL(glGenVertexArrays(1, &skyboxVAO));
		GLCALL(glGenBuffers(1, &skyboxVBO));
		GLCALL(glBindVertexArray(skyboxVAO));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) *skyboxVertices.size(), &skyboxVertices, GL_STATIC_DRAW));
		GLCALL(glEnableVertexAttribArray(0));
		GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	}

	void Render();
};

#define GEN_GETTER(TYPE, FUNC_NAME, VALUE)\
TYPE Get##FUNC_NAME(){return VALUE;}\

class SystemRenderer {

public:



	//GEN_GETTER(RenderSpec&, ActSpec, *ActSpec);
	//GEN_GETTER(MeshRenderer&, ActMesh, *ActMesh);

	std::list<Camera*> camera;

	std::list<MeshRenderer*> renderers;
	std::list<Light*> lights;
	Camera & GetCamera() {
		return *camera.front();
	}
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

				if (MeshRenderer *ren = dynamic_cast<MeshRenderer*>((*ii))) {
					renderers.push_back(ren);
					continue;
				}
			}
		}
	}
};

void CubeMap::Render() {
	//GLCALL(glDepthFunc(GL_LEQUAL));  // change depth function so depth test passes when values are equal to depth buffer's content
	//// skybox cube
	//GLCALL(glBindVertexArray(skyboxVAO));
	//shader->use();
	//shader->setInt("skybox", 0);
	//GLCALL(glActiveTexture(GL_TEXTURE0));
	//GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, tex->id));
	//GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));
	//GLCALL(glBindVertexArray(0));
	//GLCALL(glDepthFunc(GL_LESS));
	// set depth function back to default
}

#pragma region Models

class SubMesh : public Asset {
public:
	unsigned int VAO = 0;
	VectorGl <Vec3> pos;
	VectorGl <Vec3> norm;
	VectorGl <Vec2> uv;

	size_t nIndices = 0;
	size_t nUV = 0;
	size_t nPos = 0;
	size_t nNorm = 0;
	Mesh &group;
	std::array<Vec3, 2> AABB;
	SubMesh(Mesh& g, std::string n)
		: group(g), Asset(n)
	{
	}
	Mesh& Read(std::istream& file, int&);
};

class Material;

class ShaderProgram;

//http://paulbourke.net/dataformats/mtl/
class Material : objl::Material{

};





//http://paulbourke.net/dataformats/obj/
class Mesh : public Asset, virtual public  objl::Loader {
private:
public:
	std::list<MeshRenderer*> registered;

	void Register(MeshRenderer* a) {
		registered.push_back(a);
	}

	Mesh(const std::string path)
		: Asset(path)
	{
		std::ostream& file = std::cout;
		if (LoadFile(path)) {

			// Go through each loaded mesh and out its contents
			for (int i = 0; i < LoadedMeshes.size(); i++)
			{
				// Copy one of the loaded meshes to be our current mesh
				objl::Mesh curMesh = LoadedMeshes[i];

				//// Print Mesh Name
				//file << "Mesh " << i << ": " << curMesh.MeshName << "\n";

				//// Print Vertices
				//file << "Vertices:\n";

				//// Go through each vertex and print its number,
				////  position, normal, and texture coordinate
				//for (int j = 0; j < curMesh.Vertices.size(); j++)
				//{
				//	file << "V" << j << ": " <<
				//		"P(" << curMesh.Vertices[j].Position.X << ", " << curMesh.Vertices[j].Position.Y << ", " << curMesh.Vertices[j].Position.Z << ") " <<
				//		"N(" << curMesh.Vertices[j].Normal.X << ", " << curMesh.Vertices[j].Normal.Y << ", " << curMesh.Vertices[j].Normal.Z << ") " <<
				//		"TC(" << curMesh.Vertices[j].TextureCoordinate.X << ", " << curMesh.Vertices[j].TextureCoordinate.Y << ")\n";
				//}

				//// Print Indices
				//file << "Indices:\n";

				//// Go through every 3rd index and print the
				////	triangle that these indices represent
				//for (int j = 0; j < curMesh.Indices.size(); j += 3)
				//{
				//	if (j <= 0 || (j + 1) <= 0 || (j + 2) <= 0) {
				//		__debugbreak();
				//	}
				//	file << "T" << j / 3 << ": " << curMesh.Indices[j] << ", " << curMesh.Indices[j + 1] << ", " << curMesh.Indices[j + 2] << "\n";
				//}

				//// Print Material
				//file << "Material: " << curMesh.MeshMaterial.name << "\n";
				//file << "Ambient Color: " << curMesh.MeshMaterial.Ka.X << ", " << curMesh.MeshMaterial.Ka.Y << ", " << curMesh.MeshMaterial.Ka.Z << "\n";
				//file << "Diffuse Color: " << curMesh.MeshMaterial.Kd.X << ", " << curMesh.MeshMaterial.Kd.Y << ", " << curMesh.MeshMaterial.Kd.Z << "\n";
				//file << "Specular Color: " << curMesh.MeshMaterial.Ks.X << ", " << curMesh.MeshMaterial.Ks.Y << ", " << curMesh.MeshMaterial.Ks.Z << "\n";
				//file << "Specular Exponent: " << curMesh.MeshMaterial.Ns << "\n";
				//file << "Optical Density: " << curMesh.MeshMaterial.Ni << "\n";
				//file << "Dissolve: " << curMesh.MeshMaterial.d << "\n";
				//file << "Illumination: " << curMesh.MeshMaterial.illum << "\n";
				//file << "Ambient Texture Map: " << curMesh.MeshMaterial.map_Ka << "\n";
				//file << "Diffuse Texture Map: " << curMesh.MeshMaterial.map_Kd << "\n";
				//file << "Specular Texture Map: " << curMesh.MeshMaterial.map_Ks << "\n";
				//file << "Alpha Texture Map: " << curMesh.MeshMaterial.map_d << "\n";
				//file << "Bump Map: " << curMesh.MeshMaterial.map_bump << "\n";
				
				GLCALL(glGenVertexArrays(1, &curMesh.VAO));
				GLCALL(glGenBuffers(1, &curMesh.VBO));
				GLCALL(glGenBuffers(1, &curMesh.EBO));


				GLCALL(glBindVertexArray(curMesh.VAO));
				{	

					GLCALL(glBindBuffer(GL_ARRAY_BUFFER, curMesh.VBO));
					glBufferData(GL_ARRAY_BUFFER, curMesh.Vertices.size() * sizeof(objl::Vertex), &curMesh.Vertices[0], GL_STATIC_DRAW);
					{

						GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)offsetof(Vertex, Position)));
						GLCALL(glEnableVertexAttribArray(0));

						GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)offsetof(Vertex, Normal)));
						GLCALL(glEnableVertexAttribArray(1));

						GLCALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)offsetof(Vertex, BiTangent)));
						GLCALL(glEnableVertexAttribArray(2));

						GLCALL(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)offsetof(Vertex, TextureCoordinate)));
						GLCALL(glEnableVertexAttribArray(3));
					}

					GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curMesh.EBO));
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, curMesh.Indices.size() * sizeof(unsigned int), &curMesh.Indices[0], GL_STATIC_DRAW);
					{
						GLCALL(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)0));
						GLCALL(glEnableVertexAttribArray(4));

					}
				}

				GLCALL(glBindVertexArray(0));
				// Leave a space to separate from the next mesh
				file << "\n";
			}

			// Close File
			//file.close();
		}
		// If not output an error
		else
		{
			file << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";
		}
	}

	static void ProcessIndex(iVec3 &f, const size_t numVertex) {
		for (int ii = 0; ii < 3; ii++) {
			int& index = f[ii];
			if (index > 0) {
				index--;
			}
			else {
				index = (unsigned int)(numVertex + index);
			}
		}
	}

};


Mesh& SubMesh::Read(std::istream& file, int &actMat) {
	SubMesh& mesh = *this;
	Vec3 v;
	Vec3 vt;
	Vec3 vn;

	iVec3 f;
	iVec3 ft;
	iVec3 fn;
	std::string tofData;

	int size = group.submesh.size();
	if ((size - 1) > 0) { // yo ya estoy agregado entonces me saco
		nIndices = group.submesh[size - 2]->nIndices; // como soy el ultimo busco el penultimo
	}


	while (!file.eof() && file.good() && file.peek() != 'o')
	{
		std::string buffer;
		std::getline(file, buffer);
		if (buffer.size() < 1) break;
		std::stringstream stream(buffer);

		stream >> tofData;

		//std::cout << buffer << std::endl;
		if (tofData[0] == 'v')
		{
			if (tofData.size() > 1) {
				switch (tofData[1])
				{
				case 'n':
				{
					nNorm++;
					stream >> vn;
					group.tempNormal.push_back(vn);
					//DEBUG_PRINT(std::cout << "(" << vn[0] << ", " << vn[1] << ", " << vn[2] << ")" << std::endl);
				}

				break;

				case 't':
					// while(!stream.eof()) // por ahora no triangula
				{
					nUV++;
					stream >> vt[0] >> vt[1];
					group.tempUV.push_back(vt);
					//DEBUG_PRINT(std::cout << "(" << vt[0] << ", " << vt[1] << ")" << std::endl);
				}
				break;

				default:
					throw std::exception("not supported, wtf");
					break;
				}
			}
			else
			{
				// Position
				// while(!stream.eof()) //por ahora no triangula
				{
					//group.materialOrderForRender[actMat].nPos++;

					nPos++;
					stream >> v;
					group.pos.push_back(v);
					//DEBUG_PRINT(std::cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")" << std::endl);
				}
			}
		}
		else if (tofData == "f")
		{
			PF_ASSERT(actMat < group.materialOrderForRender.size());
			group.materialOrderForRender[actMat].quantityFaces++;
			nIndices++;

			auto posInit = stream.tellg();
			stream >> buffer;      // EXTRAIGO UN TOKEN PARA VER QUE TIPO DE CARA VIENE ej de buffer 6/1/1
			stream.seekg(posInit); // luego hago como si no hubiera leido nada del stream

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

			//DEBUG_PRINT(std::cout << hasDoubleSlash << " " << countDeSlash << std::endl);

			//std::vector<std::string> tokens;

			switch (countDeSlash)
			{
			case 0: // solo caras
			{
				stream >> f;

				group.iPos.push_back(f);
				//DEBUG_PRINT(std::cout << "(" << f[0] << ", " << f[1] << ", " << f[2] << ")" << std::endl);
			}
			break;
			case 1: // caras y texturas
			{
				for (int ii = 0; ii < 3; ii++)
				{
					stream >> buffer;
					sscanf_s(buffer.c_str(), "%d/%d", &f[ii], &ft[ii]);

					//DEBUG_PRINT(std::cout << "(" << f[ii] << ", " << ft[ii] << ")" << std::endl);
				}

				group.iPos.push_back(f);
				group.tempiUV.push_back(ft);
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
						//DEBUG_PRINT(std::cout << "(" << f[ii] << ", " << fn[ii] << ")" << std::endl);
					}

					group.iPos.push_back(f);
					group.tempiNormal.push_back(fn);
				}
				else
				{ //caras texturas y normales
					for (int ii = 0; ii < 3; ii++)
					{
						stream >> buffer;
						sscanf_s(buffer.c_str(), "%d/%d/%d", &f[ii], &ft[ii], &fn[ii]);
						//DEBUG_PRINT(std::cout << "(" << f[ii] << ", " << ft[ii] << ", " << fn[ii] << ")" << std::endl);
					}

					group.iPos.push_back(f);
					group.tempiNormal.push_back(fn);
					group.tempiUV.push_back(ft);
				}
			}

			break;
			}
		}
		else if (tofData == "usemtl")
		{
			stream >> buffer;// pongo el nombre del material
							 //mesh->materials.push_back(materials[buffer]);
			actMat++;

			group.materialOrderForRender.push_back(RenderSpec(*group.mtl->materials[buffer]));

			group.materialOrderForRender[actMat].init = nIndices;
		}
	}
}

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

void MeshRenderer::HandleEvent(const SDL_Event &e) {
	//if (e.type == SDL_EventType::SDL_KEYDOWN) {
	//	if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_R) {
	//		for each (auto pair in mesh.mtl->materials)
	//		{
	//			pair.second->shader->ReCompile();
	//		}
	//	}
	//}
}

MeshRenderer::MeshRenderer(COMP_PARAMS, Mesh &m) INIT_COMP("mESH rENDEERER"), mesh(m) {
	mesh.Register(this);
}

bool Component::Enabled() {
	return enabled;// && gameobject.IsEnabled();;

}


ShaderProgram* ActShader;
MeshRenderer* ActRenderer;

