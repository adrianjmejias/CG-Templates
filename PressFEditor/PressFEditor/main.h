#pragma once
#include <PressF/PressF.h>
#include "Components/Components.h"

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
// Fuego, humo, explosiones o lluvia(o cualquier efecto diseñado mediante
//	un sistema de partículas).
//	* Implementar al menos una técnica de post - procesamiento :
//	* Bloom.
//	* Heat Haze(Distorsión por calor)
//	* Motion Blur.
//	* Depth of Field.
//	* Vidrio difuso o sucio
//	* Otra técnica propuesta por usted(sujeta a previa revisión).
//	* Implementar al menos una técnica en screen space :
// SSAO.
// SSR.
// Otra técnica propuesta por usted(sujeta a previa revisión).
//- Las funciones del API deben ser demostradas a través de una aplicación aparte.
//* Consideraciones
//- Implementar el API como una librería(estática, dinámica y / o como un archivo
//	de cabecera).
//	- Documentar el API con Doxygen(salida en formato HTML).
//	- En caso de no poder implementar un técnica de screen space, puede
//	implementar al menos 3 técnicas de post - procesamiento para compezar la
//	calificación.