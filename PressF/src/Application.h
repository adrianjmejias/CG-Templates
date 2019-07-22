#pragma once


#include "types.h"
#include "Model.h"




class Application
{
public:

	unsigned int GLOBAL_ID;
	
	SDL_Window *win;
	SDL_GLContext glContext;
	int win_width;
	int win_heigth;
	
	int mouse_lastPosX;
	int mouse_lastPosY;
	int mouse_deltaX;
	int mouse_deltaY;
	
	unsigned long long NOW, LAST;
	double deltaTime;
	
	bool running;
	bool captureMouse = false;





	struct nk_context *ctx;
	struct nk_colorf bg { 0.10f, 0.18f, 0.24f, 1.0f };

public:



	std::vector<Model> models;
	void Setup(const std::vector<std::string>&, const std::vector<std::string>&);
	void MainLoop();
	void HandleEvents();
	void UILoop();
	void UpdateLoop();
	void RenderLoop();
	Application();
	~Application();
};

