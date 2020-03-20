#include "PressF/pch.h"
#include "MeshRenderer.h"


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
	}
	void MeshRenderer::OnDisable()
	{
	}
}


