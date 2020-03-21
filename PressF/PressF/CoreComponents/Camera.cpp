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
		ImGuiIO& io = ImGui::GetIO();

		bool didPrint = false;


        for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) 
			if (io.KeysDownDuration[i] >= 0.0f)
			{
				//if (io.KeysDownDuration[ii] >= 0.0f) { ImGui::SameLine(); ImGui::Text("%d (%.02f secs)", i, io.KeysDownDuration[i]); }

				std::cout <<i <<" "<< io.KeysDown[i] << "    |";
				didPrint = true;
			}

		if (didPrint)
			std::cout << std::endl;


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
