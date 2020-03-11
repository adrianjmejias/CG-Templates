#pragma once
#include "Transform.h"


namespace PF
{

#define COMP_PARAMS GameObject* go, Transform *t
#define COMP_INIT : Component(go,t)


	class Component
	{
	public:
		int enabled = 1;
		GameObject* gameObject;
		Transform* transform;
		Component();// COMP_PARAMS
		virtual void Update(const ImGuiIO& io) = 0;
		virtual void HandleEvent(const SDL_Event& e) = 0;
		~Component();
	};

	class GameObject
	{

		friend class Application;
		unsigned int id = 0;
	public:
		std::string name = "No name";
		std::vector<Owns<Component>> components;
		Transform transform;
		GameObject();
		//GameObject(const std::string& _name);
		GameObject(const std::string n);
		~GameObject();

		template <typename TT, typename ...Args>
		TT* AddComponent(Args&&... params);
		template <typename TT>
		TT* GetComponent();

		Component* AddComponent(Component* comp);


		void Update(const ImGuiIO& io)
		{
			for (auto &c : components)
			{

				PF_ASSERT(c.get() != nullptr && " components shouldnt be null");
				c->Update(io);
			}
		}
	};

	template<typename TT, typename ...Args>
	TT * GameObject::AddComponent(Args && ...params) {
		TT* comp = new TT(/*this, &transform,*/ std::forward<Args>(params)...);
		comp->transform = &transform;
		comp->gameObject = this;
		components.emplace_back(comp);
		return comp;
	}

	template<typename TT>
	TT * GameObject::GetComponent() {
		for (Component* comp : components)
		{
			if (TT* t = dynamic_cast<TT*>(comp)) {
				return t;
			}
		}
		return nullptr;
	}
}
