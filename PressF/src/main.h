#pragma once

#include <glad\glad.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_opengl.h>
#include <spdlog\spdlog.h>
#include <spdlog\sinks\basic_file_sink.h>
#include <spdlog\sinks\stdout_color_sinks.h>


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

#define COMP_PARAMS GameObject &o, Transform &t
#define INIT_COMP(n) :Component(o,t, n)
#define PF_NK(a) a ? NK_MAXIMIZED: NK_MINIMIZED

std::unique_ptr<SystemRenderer> renderer;
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
	//template <typename TT> using uptr<TT> = std::unique_ptr<TT>;
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



#include "Loaders.h"



std::vector<GameObject*> objects;

class AssetManager {
public:
	std::list<Asset> assets;
};

class Asset {
public:
	std::string path;
	std::string name;
	
};

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
	Vec3 position{0, 0, 0};
	Vec3 rotation{0, 0, 0 };
	Vec3 scale{1, 1, 1};
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
	GameObject(std::string n): transform(*this) {
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

				if (nk_tree_push(ctx, NK_TREE_TAB, comp->name.data(), static_cast<nk_collapse_states>(comp->openUI))){

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
		TT* comp= new TT(*this, transform, std::forward<Args>(params)...);
		components.push_back(comp);
		return *comp;
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


	class Renderer : public Component{

	public:
		virtual void Bind() = 0;
	};

	class MeshRenderer : public Renderer {
	public:
		Mesh& mesh;
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
		Color kA{1,1,1,1};
		Vec3 kD{0,0,1};
		Vec3 kS{1,0,0};
		Vec3 kE{0,0,0};


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

	protected:
		Light(COMP_PARAMS, std::string n) INIT_COMP(n)
		{

		}
	};

	class PointLight : public Light {
	public:
		Vec3 attenuation;

		PointLight(LIGHT_PARAMS) INIT_LIGHT("PointLight")
		{
			this->type = LightType::POINT;
		}
		// Inherited via Light
		virtual void Update() override {
			PF_INFO("PointLight running");
		}

		virtual void Bind() override {

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
		SpotLight(LIGHT_PARAMS)  INIT_LIGHT("SpotLight")
		{
			this->type = LightType::SPOTLIGHT;

		}
		// Inherited via Light
		virtual void Update() override {
			PF_INFO("SpotLight running");

		}

		virtual void Bind() override {

		}

		virtual void UI() override{
			Light::UI();
			innerAngle = nk_propertyf(ctx, "innerAngle", 0, innerAngle, outterAngle, 0.01f, 0.005f);
			outterAngle = nk_propertyf(ctx, "outterAngle", innerAngle, outterAngle, 180, 0.01f, 0.005f);
		}
	};
	
	class DirectionalLight : public Light {
	public:
		DirectionalLight(LIGHT_PARAMS)  INIT_LIGHT("DirectionalLight")
		{
			this->type = LightType::DIRECTIONAL;

		}
		// Inherited via Light
		virtual void Update() override {
			PF_INFO("DirectionalLight running");


		}
		virtual void Bind() override {

		}
		virtual void UI() override {
			Light::UI();
		}
	};

#pragma endregion Light




#pragma endregion Components

//#define STEAL(TT, stolenCollection, ii) if (TT *___tt = dynamic_cast<TT*>(stolenCollection[ii])) {
//
//	}

class SystemRenderer {
public:
	std::list<Light*> lights;
	std::list<Camera*> camera;
	std::list<Renderer*> renderers;
	std::unique_ptr<CubeMap> cubemap;
public:
	void Steal(std::vector<GameObject> gos) {

		for each (GameObject go in gos)
		{
			for (auto ii = go.components.begin(); ii != go.components.end(); ii++)
			{
				
				if (Light *light = dynamic_cast<Light*>((*ii))) {
					lights.push_back(light);
					go.components.erase(ii);
					continue;
				}
			}
		}

	}
};