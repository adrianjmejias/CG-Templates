

#pragma once
#include <PressF/PressF.h>
#include "Components/Components.h"
#include <iomanip>
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


class Editor
{
	PF::Engine engine;
	ImGui::FileBrowser fbrowser;

public: 
	Editor() = default;
	void Init();
	void LoopImGui();
};