#include "PressF/pch.h"
#include "Component.h"


namespace PF
{
	Component::Component() //COMP_PARAMS : transform(t), gameObject(go)
	{
	}

	Component::~Component()
	{
		//std::cout << "Bye Component" << std::endl;
	}
	void Component::SetActive(bool newState)
	{
		if (enabled != newState)
		{
			enabled = newState;
			OnActiveStateChange();	
		}
	}

	void Component::OnActiveStateChange()
	{
		if (enabled) {
			OnEnable();
		}
		else {
			OnDisable();
		}
	}

}