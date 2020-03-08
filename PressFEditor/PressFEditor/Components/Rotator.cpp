#include "Rotator.h"

void Rotator::Update(const ImGuiIO& io)
{
	transform->Rotate(speed * io.DeltaTime);


	PF_INFO("Rotation : {0}", transform->GetRotation().z);
}

void Rotator::HandleEvent(const SDL_Event& e)
{
}
