#pragma once
#include "Transform.h"
#include "AssetsManagement/Asset.h"

namespace PF
{

#define COMP_PARAMS GameObject* go, Transform *t
#define COMP_INIT : Component(go,t)


	class Component: Serializable
	{
		int enabled = 0;
	public:

		void SetActive(bool newState)
		{
			if (enabled != newState)
			{
				enabled = newState;
				if (newState)
				{
					OnEnable();
				}
				else
				{
					OnDisable();
				}
			}
		}
		GameObject* gameObject;
		Transform* transform;
		Component();// COMP_PARAMS
		virtual void Update(const ImGuiIO& io) = 0;
		virtual void HandleEvent(const SDL_Event& e) = 0;
		virtual void OnEnable() = 0;
		virtual void OnDisable() = 0;
		~Component();

		virtual json Serialize() = 0;
	};

	class GameObject: Asset
	{

		friend class Application;
	public:
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

		json Serialize() override;

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
