#include "PressF/pch.h"
#include "Renderer.h"


namespace PF
{
	Owns<Renderer> Renderer::instance = nullptr;

	Renderer* Renderer::GetInstance()
	{
		if (instance.get() == nullptr)
		{
			instance.reset(new Renderer());
		}
		return instance.get();
	}

	void Renderer::RegisterMesh(MeshRenderer* mesh)
	{
		objects[mesh->mesh].push_back(mesh);
	}

	void Renderer::UnRegisterMesh(MeshRenderer* mesh)
	{
		auto mrList = objects[mesh->mesh];
		//std::remove(mrList.begin(), mrList.end(), mesh);

		auto it = std::find(mrList.begin(), mrList.end(), mesh);

		if (it != mrList.end())
		{
			mrList.erase(it);
		}
	}

	void Renderer::Render()
	{
		for (auto [mesh, mrList] : objects)
		{
			mesh->Bind();
			for (auto mr : mrList)
			{
				mr->mat->Bind();
				mr->mat->shader->SetUniform("model", mr->transform->GetAccumulated());
				mesh->Render();
			}
		}
	}

}