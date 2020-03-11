#pragma once
#include "../Component.h"
#include "../Rendering/Mesh.h"



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
	};
}
