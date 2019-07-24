#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "core.h"


class Component
{
public:
	int enabled = 0;
	GameObject& gameObject;
	Transform& transform;
	Component(GameObject& go, Transform& t);
	virtual void Update() = 0;
	virtual void HandleEvent(const SDL_Event &e) = 0;

	~Component();
};

