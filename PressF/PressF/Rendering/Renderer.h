#pragma once
#include "../pch.h"
#include "../CoreComponents/CoreComponents.h"

namespace PF
{
	struct Renderer
	{
		static Owns<Renderer> instance;
		static Renderer* GetInstance()
		{
			if (instance.get() == nullptr)
			{
				instance.reset(new Renderer());
			}
			return instance.get();
		}


		std::unordered_map<GPUMesh*, std::list<MeshRenderer*>> objects;
		void RegisterMesh(MeshRenderer * mesh)
		{
			objects[mesh->mesh].push_back(mesh);
		}

		void UnRegisterMesh(MeshRenderer *mesh)
		{
			auto mrList = objects[mesh->mesh];
			//std::remove(mrList.begin(), mrList.end(), mesh);
			mrList.erase(std::find(mrList.begin(), mrList.end(), mesh));
		}

		void RegisterCamera()
		{

		}

		void UnRegisterCamera()
		{

		}


		void Render()
		{
			for (auto [mesh, mrList] : objects)
			{
				mesh->Bind();
				for (auto mr : mrList)
				{
					mr->mat->Bind();
					mesh->Render();
				}
			}
		}

		Ref<Camera> mainCamera = nullptr;
	};
}