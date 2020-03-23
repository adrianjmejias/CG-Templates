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

	void Renderer::RegisterCamera(Camera* cam)
	{
		cameras.push(cam);
	}

	void Renderer::UnRegisterCamera(Camera* cam)
	{
		std::list<Camera*> cams;

		// esto esta horrible, arreglar despues
		while (!cameras.empty())
		{
			Camera* c = cameras.top();

			if (c != cam)
			{
				cams.push_back(c);
			}
		}

		while (!cams.empty())
		{
			cameras.push(cams.front());
			cams.pop_front();
		}
	}

	void Renderer::Render()
	{

		Camera *c = cameras.top();
		c->transform->TryGetClean();
		const Mat4& projection = c->GetProjectionMatrix();
		const Mat4& view = c->GetViewMatrix();

		for (auto [mesh, mrList] : objects)
		{
			mesh->Bind();
			for (auto mr : mrList)
			{
				mr->mat->Bind();
				mr->mat->shader->SetUniform("model", mr->transform->GetAccumulated());
				mr->mat->shader->SetUniform("view", view);
				mr->mat->shader->SetUniform("projection", projection);
				mesh->Render();
				//mr->mat->shader->SetUniform("view", c->());
				
			}
		}
	}

}