#pragma once
#include"PressF/pch.h"
#include "Transform.h"

namespace PF
{
	class GameObject;

	class Component : Serializable
	{
		bool enabled = true;
	public:
		void SetActive(bool newState);
		void OnActiveStateChange();

		GameObject* gameObject;
		Transform* transform;
		Component();// COMP_PARAMS
		virtual void Update(const ImGuiIO& io) = 0;
		virtual void HandleEvent(const SDL_Event& e) = 0;
		virtual void OnEnable() = 0;
		virtual void OnDisable() = 0;
		virtual void Start() = 0;
		virtual void ImGui() = 0;
		~Component();
		virtual json Serialize() = 0;
	};

	//void ImGuiRender(const Component* c)
	//{
	//	if (ImGui::TreeNode(typeid(Rotator).name()))
	//	{

	//	}

	//}
}