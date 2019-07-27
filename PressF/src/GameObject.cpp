#include "GameObject.h"
extern unsigned int GLOBAL_ID;


GameObject::GameObject() : transform(*this)
{
	id = GLOBAL_ID++;
}

GameObject::GameObject(const std::string n) : transform(*this), name(n) {}

GameObject::~GameObject()
{
	std::cout << "Bye GameObject";

}