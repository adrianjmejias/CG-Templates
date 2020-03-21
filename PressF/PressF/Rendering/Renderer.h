#pragma once
#include "../pch.h"

#include "PressF/CoreComponents/Camera.h"
#include "PressF/CoreComponents/MeshRenderer.h"

namespace PF
{
	static std::function<bool(Camera*,Camera*)> cameraComparer = [](Camera* l, Camera* r) -> bool { return *(l->priority) < *(r->priority); };
	struct Renderer
	{
		static Owns<Renderer> instance;
		std::unordered_map<GPUMesh*, std::list<MeshRenderer*>> objects;
		std::priority_queue < Camera*, std::vector<Camera*>, decltype(cameraComparer)> cameras{ cameraComparer };

		static Renderer* GetInstance();

		void RegisterMesh(MeshRenderer* mesh);

		void UnRegisterMesh(MeshRenderer* mesh);

		void RegisterCamera(Camera *cam)
		{
			cameras.push(cam);
		}

		void UnRegisterCamera(Camera *cam)
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

		void Render();

		Ref<Camera> mainCamera = nullptr;
	};
}