#include "main.h"



//Implementar un API(Application Programming Interface) para creación de
//escenas 3D con diversos efectos.
//* Funciones del API :
//-Crear escena.
//- Agregar Modelos(Con texturas e iluminación).
//- Navegar escena. 
//- Salvar / Cargar escena.
//- Detección de colisiones.
//- Activar / Desactivar efectos :



// Estereoscopía.


// Necesitamos la distancia intraocular (IOD)
// distancia del plano imagen 










// Fuego, humo, explosiones o lluvia(o cualquier efecto diseñado mediante
//	un sistema de partículas).
//	* Bloom.
	
	//-- Hacer el render normal
	//-- Hacer un render donde solo esten los objetos que brillen, aplicarle un blur
	//-- combinar ambos render





//	* Implementar al menos una técnica en screen space :
// SSAO.
// SSR.



//	* Implementar al menos una técnica de post - procesamiento :
//	* Heat Haze(Distorsión por calor)
//	* Motion Blur.
//	* Depth of Field.
//	* Vidrio difuso o sucio
//	* Otra técnica propuesta por usted(sujeta a previa revisión).
// Otra técnica propuesta por usted(sujeta a previa revisión).
//- Las funciones del API deben ser demostradas a través de una aplicación aparte.
//* Consideraciones
//	- Documentar el API con Doxygen(salida en formato HTML).
//	- En caso de no poder implementar un técnica de screen space, puede
//	implementar al menos 3 técnicas de post - procesamiento para compezar la
//	calificación.


int main() {
	Editor editor;

	editor.Init();

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

	return 0;
}