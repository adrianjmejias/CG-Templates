#pragma once
#include "PressF/AssetsManagement/Asset.h"
#include "Transform.h"
#include "Component.h"

namespace PF
{

	class GameObject: Asset
	{
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

		void Update(const ImGuiIO& io);

		void OnEnable();


		void ImGui();

		void OnDisable();

		void Start();
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
