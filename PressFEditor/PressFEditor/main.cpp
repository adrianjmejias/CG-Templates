#include "main.h"



//Implementar un API(Application Programming Interface) para creaci�n de
//escenas 3D con diversos efectos.
//* Funciones del API :
//-Crear escena.
//- Agregar Modelos(Con texturas e iluminaci�n).
//- Navegar escena. 
//- Salvar / Cargar escena.
//- Detecci�n de colisiones.
//- Activar / Desactivar efectos :



// Estereoscop�a.


// Necesitamos la distancia intraocular (IOD)
// distancia del plano imagen 










// Fuego, humo, explosiones o lluvia(o cualquier efecto dise�ado mediante
//	un sistema de part�culas).
//	* Bloom.
	
	//-- Hacer el render normal
	//-- Hacer un render donde solo esten los objetos que brillen, aplicarle un blur
	//-- combinar ambos render





//	* Implementar al menos una t�cnica en screen space :
// SSAO.
// SSR.



//	* Implementar al menos una t�cnica de post - procesamiento :
//	* Heat Haze(Distorsi�n por calor)
//	* Motion Blur.
//	* Depth of Field.
//	* Vidrio difuso o sucio
//	* Otra t�cnica propuesta por usted(sujeta a previa revisi�n).
// Otra t�cnica propuesta por usted(sujeta a previa revisi�n).
//- Las funciones del API deben ser demostradas a trav�s de una aplicaci�n aparte.
//* Consideraciones
//	- Documentar el API con Doxygen(salida en formato HTML).
//	- En caso de no poder implementar un t�cnica de screen space, puede
//	implementar al menos 3 t�cnicas de post - procesamiento para compezar la
//	calificaci�n.


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