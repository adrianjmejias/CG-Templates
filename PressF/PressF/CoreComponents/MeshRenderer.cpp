#include "PressF/pch.h"
#include "MeshRenderer.h"
#include "../Rendering/Renderer.h"

namespace PF
{
	void MeshRenderer::Update(const ImGuiIO& io)
	{
		//mesh->Bind();
		////mat->shader->Use();
		//mat->Bind();
		//mat->shader->SetUniform("model", transform->GetAccumulated());
		//mesh->Render();
	}
	json MeshRenderer::Serialize() {
		json j;

		return j;
	}
	void MeshRenderer::OnEnable()
	{
		SetRenderMode(renderMode);
		ApplyRenderMode();
	}
	void MeshRenderer::OnDisable()
	{
		//renderMask.SetMode(MeshRenderMode::None);

		Renderer::GetInstance()->UnRegisterMesh(this);
	}
	void MeshRenderer::Start()
	{
		
	}
	void MeshRenderer::ImGui()
	{
	}

	void MeshRenderer::SetRenderMode(MeshRenderMode mode)
	{
		renderMode = mode;
		renderMask.SetMode(renderMode);
		//ApplyRenderMode();
	}

	void MeshRenderer::SetGPUMesh(GPUMesh* m)
	{
		mesh = m;
		mat = m->defaultMaterial;
	}

	void MeshRenderer::ApplyRenderMode()
	{
		Renderer::GetInstance()->RegisterMesh(this, renderMask);
	}
}


