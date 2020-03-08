#pragma once

//#include "core.h"

//#include "Camera.h"
//#include "Light.h"
//#include "MeshRenderer.h"

#include "OBJ_Loader.h"
//#include "MeshRenderer.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Component.h"
#include "GameObject.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <SDL.h>
#include <imgui.h>
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#define ADD_MAP(NAME)\
		if (!mat.NAME.empty() && (texturesLoaded.find(mat.NAME) == texturesLoaded.end())) {\
			texturesLoaded[mat.NAME] = Texture::TextureFromFile(mat.NAME.c_str(), basePath);\
		}\



#define ADD_MAP_TO_MATERIAL(NAME)\
	if (!mesh.mat.NAME.empty()) {\
		mesh.mat.s##NAME = texturesLoaded.at(mesh.mat.NAME);\
	}\


class Application
{
public:
	//Every object created must have a new id
	//unsigned int GLOBAL_ID; //replaced with extern
	SDL_Window *win;
	SDL_GLContext glContext;
	int win_width = 800;
	int win_heigth = 600;
	//static double deltaTime;
	int mouse_lastPosX;
	int mouse_lastPosY;
	int mouse_deltaX;
	int mouse_deltaY;

	unsigned long long NOW, LAST;

	float IOR_BG = 1;
	float heightScale = 2.f;
	float minLayers = 8;
	float maxLayers = 200;

	bool running;
	bool captureMouse = true;
	bool show_demo_window = true;
	bool show_another_window = true;
	bool KeyPressed[256];
	Vec4 bgColor{ 0.2f,0.2f,0.2f, 1 };

	//GameObject* actGO = nullptr;
	int actCam = 0;
	std::vector<ShaderProgram*> shaders;
	//std::vector<Light*> LIGHTS;
	//Model* lightsModel = nullptr;

	//std::vector < Camera*> orderedCameras;
	//std::vector < Camera*> cameras;
	//std::vector<MeshRenderer*> renderers;
	//std::vector<Model*> models;
	//std::vector<Model*> modelsDebug;
	//std::vector<GameObject *> rootNodes;
	//std::vector<Material *> materials;
	//std::map<std::string, Shader*> shadersLoaded;
	//std::map<std::string, Material*> materialsLoaded;
	//std::map<std::string, Texture*> texturesLoaded;
	//CubeMap *cubeMap;
	//std::unique_ptr<FrameBuffer> depthFB;
	unsigned int VAO_PLANE, VBO_PLANE;
	bool renderPlane = false;
	bool renderScene = true;
	//FlyingController * fc;
	ShaderProgram* depthPlaneShader = nullptr;
	Vec4 orthoSides{-80,80,-80,80};
	Vec2 clippingPlane{ 1.f,400.f };
	void LoadModels(const std::vector<std::string>& objPaths);

public:



	//static double DeltaTime() { return deltaTime; }




	//static Model* GLCreate(objl::Loader & fullModel);
	//void Steal(Component *);
	void LoopEvents();
	//Model* GLCreate(objl::Loader& fullModel);
	void Steal(Component* comp);
	void Setup();
	void SetupScene();
	//Model* SetupModel(std::string objPath);
	void LoadShaders(const std::vector<std::tuple<std::string, std::string>>& shaderPaths);
	void SetupContext();


	void LoopMain();
	void LoopUI();
	void LoopUpdate();
	void LoopRender();

	/*void DrawObjects(const Mat4& view, const Mat4& projection, std::vector<const Mesh *> meshes, std::function<bool(const ShaderProgram& shader, const Material& MAT)>  PreReqs);
	void DrawObjects(const Mat4& view, const Mat4& projection, std::vector<const MeshRenderer *> meshes, std::function<bool(const ShaderProgram& shader, const Material& MAT, const MeshRenderer& mesh)>  PreReqs);*/


	Application();
	~Application();
};
