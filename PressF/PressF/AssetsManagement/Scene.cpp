#include "PressF/pch.h"
#include "Scene.h"


namespace PF
{
	void Scene::Serialize()
	{

	}
	Scene::Scene() : Scene("Main Scene")
	{
	}
	Scene::Scene(const std::string& _name) : name(_name) {}

	GameObject& Scene::AddGameObject(GameObject* go)
	{
		rootObjects.emplace_back(go);
		return *go;
	}

	void Scene::Update(const ImGuiIO& io)
	{
		for (auto& go : rootObjects)
		{
			go->Update(io);
		}
	}

	void Scene::Start()
	{
		for (auto& go : rootObjects)
		{
			for (auto& c : go->components)
			{
				c->Start();
				c->OnActiveStateChange();
			}
		}
	}
}