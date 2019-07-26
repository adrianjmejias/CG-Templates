#include "main.h"

int main() {
	Application app;

	bool capture = false;

	if (SDL_SetRelativeMouseMode(static_cast<SDL_bool>(capture)) == -1) {
		__debugbreak();
	}

	app.Setup(
		{
			"assets/models/cuboPower/simpleQuad.obj",
			//"assets/models/cuboPower/simpleCube.obj",
			//"assets/models/cuboPower/clown.obj",
			"assets/models/cuboPower/cuboPower.obj"
		}, {
			// Cook
			std::make_tuple("COOK.vert", "COOK.frag"),
			// Transparency 1
			std::make_tuple("TRANSPARENCY.vert", "TRANSPARENCY.frag"),
			// Reflection 2
			std::make_tuple("REFLECTION.vert", "REFLECTION.frag"),
			// Refraction 3
			std::make_tuple("REFRACTION.vert", "REFRACTION.frag"),
			// SHADOW FIRST PASS 4
			std::make_tuple("COOK.vert", "COOK.frag"),
			// SHADOW SECOND PASS 5
			std::make_tuple("COOK.vert", "COOK.frag"),
			// Cubemap 6
			std::make_tuple("CUBEMAP.vert", "CUBEMAP.frag"),
			// PARALLAX_MAPPING 7
			std::make_tuple("COOK.vert", "COOK.frag"),
			// NORMAL_MAPPING 8
			std::make_tuple("COOK.vert", "COOK.frag"),
			std::make_tuple("BASIC.vert", "BASIC.frag"),


		});

	app.LoopMain();

	return 0;
}