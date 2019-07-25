#pragma once
#include "Transform.h"


class FlyingController {
	Transform &t;
	float speed = 0.5f;
	float sensitivity = 1;
public:
	FlyingController(Transform& _t);
	void HandleEvent(SDL_Event e);
};


class Camera
{
public:
	Transform t;
	FlyingController fc;
	Camera();
	~Camera();
};

