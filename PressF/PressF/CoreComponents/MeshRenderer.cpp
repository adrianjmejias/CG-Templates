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
	void MeshRenderer::HandleEvent(const SDL_Event& e)
	{
	}

	json MeshRenderer::Serialize() {
		json j;

		return j;
	}
	void MeshRenderer::OnEnable()
	{
		Renderer::GetInstance()->RegisterMesh(this);
	}
	void MeshRenderer::OnDisable()
	{
		Renderer::GetInstance()->UnRegisterMesh(this);
	}
	void MeshRenderer::Start()
	{
	}
}


