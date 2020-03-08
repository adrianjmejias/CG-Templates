#include "main.h"



//Implementar un API(Application Programming Interface) para creaci�n de
//escenas 3D con diversos efectos.
//* Funciones del API :
//-Crear escena.
//- Agregar Modelos(Con texturas e iluminaci�n).
//- Navegar escena.
//- Salvar / Cargar escena.
//- Mostrar / Ocultar FPS(Frames per Second).
//- Detecci�n de colisiones.
//- Activar / Desactivar efectos :
// Estereoscop�a.
// Fuego, humo, explosiones o lluvia(o cualquier efecto dise�ado mediante
//	un sistema de part�culas).
//	* Implementar al menos una t�cnica de post - procesamiento :
//	* Bloom.
//	* Heat Haze(Distorsi�n por calor)
//	* Motion Blur.
//	* Depth of Field.
//	* Vidrio difuso o sucio
//	* Otra t�cnica propuesta por usted(sujeta a previa revisi�n).
//	* Implementar al menos una t�cnica en screen space :
// SSAO.
// SSR.
// Otra t�cnica propuesta por usted(sujeta a previa revisi�n).
//- Las funciones del API deben ser demostradas a trav�s de una aplicaci�n aparte.
//* Consideraciones
//- Implementar el API como una librer�a(est�tica, din�mica y / o como un archivo
//	de cabecera).
//	- Documentar el API con Doxygen(salida en formato HTML).
//	- En caso de no poder implementar un t�cnica de screen space, puede
//	implementar al menos 3 t�cnicas de post - procesamiento para compezar la
//	calificaci�n.
int main() {
	PF::Engine engine;
	engine.InitContext();

	//app.SetupContext();
	////app.LoadModels({});
	////app.LoadShaders({});
	////PF::Scene scene;
	//engine.LoadModels(
	//		{
	//		"assets/models/cuboPower/simpleQuad.obj",
	//		//"assets/models/cuboPower/simpleCube.obj",
	//		//"assets/models/cuboPower/clown.obj",
	//		"assets/models/cuboPower/cuboPowerblend.obj"
	//	}
	//);
	PF::ModelsManager& mm = engine.modelsManager;
	PF::Model& model = mm.LoadAsset("assets/models/cuboPower/simpleQuad.obj");
	
	PF::Scene& scene = engine.AddScene(new PF::Scene("Example scene"));

	PF::GameObject& ts1 = scene.AddGameObject(new PF::GameObject("test subject 1"));
	PF::GameObject& ts2 = scene.AddGameObject(new PF::GameObject("test subject 2"));

	ts1.AddComponent<Rotator>();

	ts2.AddComponent<ParticleSystem>()->Start();



	PF_INFO("number of gameobjects %d", scene.rootObjects.size());
	std::cout << "model loaded" << model.EBO;

	//app.LoadShaders(
	//	{
	//		// Cook
	//	//std::make_tuple("COOK.vert", "COOK.frag"),
	//	// Transparency 1
	//	//std::make_tuple("TRANSPARENCY.vert", "TRANSPARENCY.frag"),
	//	//std::make_tuple("TRANSPARENCY.vert", "TRANSPARENCY.frag"),
	//	//// Reflection 2
	//	//std::make_tuple("REFLECTION.vert", "REFLECTION.frag"),
	//	//// Refraction 3
	//	//std::make_tuple("REFRACTION.vert", "REFRACTION.frag"),
	//	//// SHADOW FIRST PASS 4
	//	//std::make_tuple("DEPTH.vert", "DEPTH.frag"),
	//	//// SHADOW SECOND PASS 5
	//	//std::make_tuple("SHADOW_MAPPING.vert", "SHADOW_MAPPING.frag"),
	//	//// Cubemap 6
	//	//std::make_tuple("CUBEMAP.vert", "CUBEMAP.frag"),
	//	//// PARALLAX_MAPPING 7
	//	//std::make_tuple("PARALLAX.vert", "PARALLAX.frag"),
	//	//std::make_tuple("PARALLAX.vert", "PARALLAX.frag"),
	//	//// NORMAL_MAPPING 8
	//	////std::make_tuple("NORMAL.vert", "NORMAL.frag"),
	//	//// NORMAL_MAPPING 9 tex disp
	//	//std::make_tuple("LIGHT.vert", "LIGHT.frag"),
	//	//std::make_tuple("LIGHT.vert", "LIGHT.frag"),
	//	//// NORMAL_MAPPING 10 tex disp
	//	////std::make_tuple("NORMAL.vert", "NORMAL.frag"),
	//	//// NORMAL_MAPPING 11 tex disp 
	//	//std::make_tuple("BASIC.vert", "BASIC.frag"),
	//	});
		



	engine.LoopMain();
	//app.LoopMain();

	while (engine.running)
	{
		engine.LoopMain();
	}

	engine.CleanContext();



	return 0;
}