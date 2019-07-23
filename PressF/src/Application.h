#pragma once

#include "types.h"

#include "Model.h"
#include "Light.h"
#include "GameObject.h"
#include "ShaderProgram.h"

class Application
{
public:
	//Every object created must have a new id
	unsigned int GLOBAL_ID;

	SDL_Window *win;
	SDL_GLContext glContext;
	int win_width = 800;
	int win_heigth = 600;

	int mouse_lastPosX;
	int mouse_lastPosY;
	int mouse_deltaX;
	int mouse_deltaY;

	unsigned long long NOW, LAST;
	double deltaTime;

	bool running;
	bool captureMouse = false;

	bool KeyPressed[256];

	ShaderProgram* shaders[30];
	std::vector<Light> LIGHTS[3];
	std::vector<Mesh> meshes;


	std::map<std::string, Shader*> shadersLoaded;
	
public:

	Mesh GLCreate(objl::Mesh & model);

	void Setup(const std::vector<std::string>&, const std::vector<std::tuple<std::string, std::string>>&);
	void MainLoop();
	void HandleEvents();
	void UILoop();
	void UpdateLoop();
	void RenderLoop();
	Application();
	~Application();
};
