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

	void Scene::OnEnable()
	{
		for (auto& go : rootObjects)
		{
			go->OnEnable();
		}
	}

	void Scene::OnDisable()
	{
		for (auto& go : rootObjects)
		{
			go->OnDisable();
		}
	}


	void Scene::Start()
	{

	}
}