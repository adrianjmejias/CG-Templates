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

		//struct nk_context *ctx;
		//struct nk_colorf bg { 0.10f, 0.18f, 0.24f, 1.0f };



		const char button_map[256] = {
		//[SDL_BUTTON_LEFT & 0xff] = MU_MOUSE_LEFT,
		//[SDL_BUTTON_RIGHT & 0xff] = MU_MOUSE_RIGHT,
		//[SDL_BUTTON_MIDDLE & 0xff] = MU_MOUSE_MIDDLE,
	};

	const char key_map[256] = {
	//	[SDLK_LSHIFT & 0xff] = MU_KEY_SHIFT,
	//	[SDLK_RSHIFT & 0xff] = MU_KEY_SHIFT,
	//	[SDLK_LCTRL & 0xff] = MU_KEY_CTRL,
	//	[SDLK_RCTRL & 0xff] = MU_KEY_CTRL,
	//	[SDLK_LALT & 0xff] = MU_KEY_ALT,
	//	[SDLK_RALT & 0xff] = MU_KEY_ALT,
	//	[SDLK_RETURN & 0xff] = MU_KEY_RETURN,
	//	[SDLK_BACKSPACE & 0xff] = MU_KEY_BACKSPACE,
	};

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

