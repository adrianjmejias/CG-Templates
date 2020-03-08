#pragma once
#include "pch.h"
#include "GameObject.h"

namespace PF
{

	class Scene
	{
	public:
		std::string name;
		std::vector<uptr<GameObject>> rootObjects;
		Scene(const std::string& _name);


		GameObject& AddGameObject(GameObject* go)
		{
			rootObjects.emplace_back(go);
			return *go;
		}

		void Update(const ImGuiIO &io)
		{
			for (auto& go : rootObjects)
			{
				go->Update(io);
			}
			
		}
	};
}