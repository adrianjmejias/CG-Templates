#include "GameObject.h"
extern unsigned int GLOBAL_ID;
GameObject::GameObject()
{
	id = GLOBAL_ID++;
}

GameObject::~GameObject()
{
	delete[] components.data();
}