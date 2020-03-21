#pragma once
#include "PressF/pch.h"
#include "PressF/EntityComponent/Component.h"
#include "PressF/Rendering/Mesh.h"



namespace PF
{
	class MeshRenderer : public Component
	{
	public:
		Ref<GPUMesh> mesh;
		Ref<Material> mat;

		// Inherited via Component
		virtual void Update(const ImGuiIO& io) override;

		virtual void HandleEvent(const SDL_Event& e) override;
		virtual json Serialize() override;


		// Inherited via Component
		virtual void OnEnable() override;

		virtual void OnDisable() override;
		
		// Inherited via Component
		virtual void Start() override;

	};
}
