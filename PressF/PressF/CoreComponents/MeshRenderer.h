#pragma once
#include "PressF/pch.h"
#include "PressF/EntityComponent/Component.h"
#include "PressF/Rendering/loglMesh.h"
#include "PressF/Rendering/MaterialSystem/Material.h"

namespace PF
{




	class MeshRenderer : public Component
	{
	public:
		Ref<GPUMesh> mesh;
		Ref<Material> mat;
		MeshRenderMode renderMode{ MeshRenderMode::Normal };
		RenderMask renderMask;
		// Inherited via Component
		virtual void Update(const ImGuiIO& io) override;


		virtual json Serialize() override;


		// Inherited via Component
		virtual void OnEnable() override;

		virtual void OnDisable() override;
		
		// Inherited via Component
		virtual void Start() override;


		// Inherited via Component
		virtual void ImGui() override;
		
		void SetRenderMode(MeshRenderMode mode);
		void ApplyRenderMode();
		void SetGPUMesh(GPUMesh* m);
	};
}
