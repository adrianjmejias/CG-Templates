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
		}, {
			// Cook
			std::make_tuple("COOK.vert", "COOK.frag"),
			// Transparency 
			std::make_tuple("TRANSPARENCY.vert", "TRANSPARENCY.frag"),
			// Reflection
			std::make_tuple("COOK.vert", "COOK.frag"),
			// Refraction
			std::make_tuple("COOK.vert", "COOK.frag"),
			// SHADOW FIRST PASS
			std::make_tuple("COOK.vert", "COOK.frag"),
			// SHADOW SECOND PASS
			std::make_tuple("COOK.vert", "COOK.frag"),
			// Cubemap
			std::make_tuple("CUBEMAP.vert", "CUBEMAP.frag"),
			// PARALLAX_MAPPING
			std::make_tuple("COOK.vert", "COOK.frag"),
			// NORMAL_MAPPING


		});

	app.MainLoop();

	return 0;
}