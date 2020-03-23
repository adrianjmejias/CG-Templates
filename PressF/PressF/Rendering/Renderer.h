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

		void RegisterCamera(Camera* cam);

		void UnRegisterCamera(Camera* cam);

		void Render();

	};
}