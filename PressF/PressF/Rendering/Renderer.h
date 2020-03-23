#pragma once
#include "../pch.h"

#include "PressF/CoreComponents/Camera.h"
#include "PressF/CoreComponents/MeshRenderer.h"
#include "PressF/CoreComponents/ParticleSystem.h"
#include "PressF/CoreComponents/Light.h"

namespace PF
{
	static std::function<bool(Camera*,Camera*)> cameraComparer = [](Camera* l, Camera* r) -> bool { return *(l->priority) < *(r->priority); };
	struct Renderer
	{
		static Owns<Renderer> instance;
		std::unordered_map<GPUMesh*, std::list<MeshRenderer*>> objects;
		std::vector<ParticleSystem*> particlesSystems;
		std::vector<Light*> lights;
		std::priority_queue < Camera*, std::vector<Camera*>, decltype(cameraComparer)> cameras{ cameraComparer };

		static Renderer* GetInstance();

		void RegisterMesh(MeshRenderer* mesh);
		void UnRegisterMesh(MeshRenderer* mesh);


		void RegisterCamera(Camera* cam);
		void UnRegisterCamera(Camera* cam);


		void RegisterParticleSystem(ParticleSystem* ps);
		void UnRegisterParticleSystem(ParticleSystem* ps);

		void RegisterLight(Light* l);
		void UnRegisterLight(Light* l);





		void Render();

	};
}