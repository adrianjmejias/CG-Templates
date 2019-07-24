#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
	delete[] components.data();
}