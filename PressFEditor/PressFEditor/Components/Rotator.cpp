#include "Rotator.h"

void Rotator::Update(const ImGuiIO& io)
{
	transform->Rotate(speed * io.DeltaTime);
	//PF_INFO("Rotation : {0}", transform->GetRotation().z);
}

void Rotator::HandleEvent(const SDL_Event& e)
{
	auto& io = ImGui::GetIO();

	if (ImGui::IsKeyDown('w'))
	{
		PF_INFO("it works");
	}
}
[[hola ]]
json Rotator::Serialize()
{
	json j;

	j["speed"] = Serialization::Serialize(speed);
	j["type"] = typeid(Rotator).name();

	return j;
}

void Rotator::OnEnable()
{
}

void Rotator::OnDisable()
{
}
