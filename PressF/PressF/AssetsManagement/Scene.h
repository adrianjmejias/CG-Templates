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
		Scene();
		Scene(const std::string& _name);


		GameObject& AddGameObject(GameObject* go);
		void Serialize();
		void Update(const ImGuiIO& io);
		void ImGui();
		void Start();
	};
}