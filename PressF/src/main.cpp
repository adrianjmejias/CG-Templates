#include "types.h"
#include "main.h"

int main() {
	Application app;



	app.Setup(
		{
			"assets/models/volumeCube.obj",
			"assets/models/transfer.obj",
			//"assets/models/screenQuad.obj",
		}, 
		{
			//1 simple texture shader
			std::make_tuple("firstPass.vert", "firstPass.frag"),
			//2 volume raycast first pass
			std::make_tuple("lastPass.vert", "lastPass.frag"),
			//3 volume raycast last pass
			std::make_tuple("renderQuad.vert", "renderQuad.frag"),
			std::make_tuple("shaderUI.vert", "shaderUI.frag"),
			//std::make_tuple("texAnim.vert", "texAnim.frag"),
		});

	app.LoopMain();

	return 0;
}