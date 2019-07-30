#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "core.h"

#define COMP_PARAMS GameObject* go, Transform *t
#define COMP_INIT : Component(go,t)

class Component
{
public:
	int enabled = 1;
	GameObject* gameObject;
	Transform* transform;
	Component(COMP_PARAMS);
	virtual void Update() = 0;
	virtual void HandleEvent(const SDL_Event &e) = 0;

	~Component();
};

