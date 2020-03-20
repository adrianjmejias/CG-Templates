#include "PressF/pch.h"
#include "GameObject.h"
namespace PF{

	GameObject::GameObject() : transform(*this)
	{
		//id = GLOBAL_ID++;
	}

	GameObject::GameObject(const std::string n) : transform(*this) {}

	GameObject::~GameObject()
	{
		//std::cout << "Bye GameObject"<<std::endl;

	}


	json GameObject::Serialize()
	{
		json j = Asset::Serialize();

		j["transform"] = transform.Serialize();
		j["components"] = json::array();
		
		int ii = 0;
		for (auto& c : components)
		{
			json ja = c->Serialize();
			j["components"].push_back(ja);
			ii++;
		}

		return j;
	}


	Component* GameObject::AddComponent(Component* comp)
	{
		PF_ASSERT(comp && "Component is null");
		if (comp->gameObject == this) {
			PF_WARN("ADDING COMPONENT TO YOURSELF");
			return comp;
		}

		GameObject* previousOwner = comp->gameObject;
		PF_ASSERT(previousOwner && "component must always belong to a gameobject. shouldn't be created by new");
		auto& comps = previousOwner->components;
		// TODO: cambiar esto por swap al final y pop back para hacerlo orden 1
		//std::remove_if(begin(components), end(components), [&](Component* c) {return c == comp; });

		Owns<Component> cptr;
 		for (int ii = 0; ii < comps.size(); ii++)
		{
			if (comps[ii].get() == comp) {
				cptr = std::move(comps[ii]);
				comps.erase(begin(comps) + ii);
				ii = comps.size() + 1;
			}
		}


		comp->gameObject = this;
		comp->transform = &transform;
		components.push_back(std::move(cptr));
		return comp;
	}





	Component::Component() //COMP_PARAMS : transform(t), gameObject(go)
	{
	}

	Component::~Component()
	{
		//std::cout << "Bye Component" << std::endl;
	}
}

