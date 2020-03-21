#include "PressF/pch.h"
#include "Camera.h"
#include "PressF/Rendering/Renderer.h"


namespace PF
{
	void Camera::Update(const ImGuiIO& io)
	{
	}

	void Camera::HandleEvent(const SDL_Event& e)
	{
	}

	json Camera::Serialize()
	{
		return json();
	}

	void Camera::OnEnable()
	{
		PF::Renderer::GetInstance()->RegisterCamera(this);
	}

	void Camera::OnDisable()
	{
		PF::Renderer::GetInstance()->UnRegisterCamera(this);
	}

	void Camera::Start()
	{
		transform->Translate(0, 0, -4);
	}
	
}
