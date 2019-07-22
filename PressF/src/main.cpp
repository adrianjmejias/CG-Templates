#define _CRT_SECURE_NO_WARNINGS
#include "main.h"



int main() {
	Application app;

	bool capture = false;

	if (SDL_SetRelativeMouseMode(static_cast<SDL_bool>(capture)) == -1) {
		__debugbreak();
	}

	app.Setup(
		{ 
			"assets/models/cuboPower/cuboPower.obj" 
		},{

		});

	app.MainLoop();
}