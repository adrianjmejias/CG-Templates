#pragma once

#include "core.h"

#include "Camera.h"
#include "Light.h"
#include "MeshRenderer.h"

struct CameraCompare
{
	bool operator() (Camera* a, Camera* b)
	{
		return a->power < b->power;
	}
};


class Application
{
public:
	//Every object created must have a new id
	unsigned int GLOBAL_ID;

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

	bool running;
	bool captureMouse = false;

	bool KeyPressed[256];
	Vec4 bgColor{0.396,0.313,0.686, 1};


	ShaderProgram* shaders[30]{nullptr};
	std::vector<Light*> LIGHTS;
	std::priority_queue < Camera*, std::vector<Camera*>, CameraCompare> cameras;
	std::vector<MeshRenderer*> renderers;
	std::vector<Model*> models;
	std::vector<GameObject *> objects;
	std::map<std::string, Shader*> shadersLoaded;
	std::map<std::string, Texture*> texturesLoaded;
	
public:



	//static double DeltaTime() { return deltaTime; }
	void GLCreate(objl::Loader & model);
	void Setup(const std::vector<std::string>&, const std::vector<std::tuple<std::string, std::string>>&);
	void SetupScene();
	void SetupModels(const std::vector<std::string>& objPathsp);
	void SetupShaders(const std::vector<std::tuple<std::string, std::string>>& shaderPaths);
	void MainLoop();
	void HandleEvents();
	void UILoop();
	void UpdateLoop();
	void RenderLoop();
	void Steal(Component *);
	Application();
	~Application();
};
