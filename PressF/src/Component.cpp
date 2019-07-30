#include "Component.h"

Component * GameObject::AddComponent(Component * comp)
{
	PF_ASSERT(comp && "Component is null");
	if (comp->gameObject == this) {
		PF_WARN("ADDING COMPONENT TO YOURSELF");
		return comp;
	}

	GameObject *previousOwner = comp->gameObject;
	PF_ASSERT(previousOwner && "component must always belong to a gameobject. shouldn't be created by new");
	std::vector<Component *> &comps = previousOwner->components;
	// TODO: cambiar esto por swap al final y pop back para hacerlo orden 1
	//std::remove_if(begin(components), end(components), [&](Component* c) {return c == comp; });
	for (int ii = 0; ii < comps.size(); ii++)
	{
		if (comps[ii] == comp) {
			comps.erase(begin(comps)+ii);
			ii = comps.size()+1;
		}
	}


	comp->gameObject = this;
	comp->transform = &transform;
	components.push_back(comp);
	return comp;
}

Component::Component(COMP_PARAMS) : transform(t), gameObject(go)
{
}

Component::~Component()
{
	std::cout << "Bye Component" << std::endl;
}
