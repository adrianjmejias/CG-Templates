#include "types.h"
#include "main.h"

int main() {
	Application app;



	app.Setup(
		{
			//"assets/models/volumeCube.obj",
			//"assets/models/transfer.obj",
			//"assets/models/screenQuad.obj",
		}, 
		{
			//1 simple texture shader
			std::make_tuple("raycast.vert", "raycast.frag"),
		});

	app.LoopMain();

	return 0;
}