#pragma once
#include "PressF/pch.h"
#include "PressF/EntityComponent/GameObject.h"

namespace PF
{

	class Scene
	{
	public:

		std::string name;
		std::vector<Owns<GameObject>> rootObjects;
		Scene(const std::string& _name);


		GameObject& AddGameObject(GameObject* go);
		void Serialize();
		Scene();
		void Update(const ImGuiIO& io);
		void OnEnable();
		void OnDisable();
		void Start();
	};
}