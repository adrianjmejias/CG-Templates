#pragma once
#include "types.h"

#include "Transform.h"

class GameObject
{
public:
	Transform transform;

	GameObject();
	~GameObject();
};
