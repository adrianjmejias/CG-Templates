#include "GameObject.h"
extern unsigned int GLOBAL_ID;


GameObject::GameObject() : transform(*this)
{
	id = GLOBAL_ID++;
	name += std::to_string(id);
}

GameObject::GameObject(const std::string n) : transform(*this), id(GLOBAL_ID++) ,name(n + std::to_string(id)) {}

GameObject::~GameObject()
{
	std::cout << "Bye GameObject"<<std::endl;

}

