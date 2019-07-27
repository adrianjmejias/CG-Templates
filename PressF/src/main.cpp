#include "main.h"

int main() {
	Application app;



	app.Setup(
		{
			"assets/models/cuboPower/simpleQuad.obj",
			//"assets/models/cuboPower/simpleCube.obj",
			//"assets/models/cuboPower/clown.obj",
			//"assets/models/cuboPower/cuboPower.obj"
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
			std::make_tuple("NORMAL.vert", "NORMAL.frag"),
			// SHADOW SECOND PASS 5
			std::make_tuple("NORMAL.vert", "NORMAL.frag"),
			// Cubemap 6
			std::make_tuple("CUBEMAP.vert", "CUBEMAP.frag"),
			// PARALLAX_MAPPING 7
			std::make_tuple("PARALLAX.vert", "PARALLAX.frag"),
			// NORMAL_MAPPING 8
			std::make_tuple("NORMAL.vert", "NORMAL.frag"),
			// NORMAL_MAPPING 9 tex disp
			std::make_tuple("COOK.vert", "COOK.frag"),
			// NORMAL_MAPPING 10 tex disp
			std::make_tuple("NORMAL.vert", "NORMAL.frag"),
			// NORMAL_MAPPING 11 tex disp 
			std::make_tuple("COOK.vert", "COOK.frag"),



		});

	app.LoopMain();

	return 0;
}