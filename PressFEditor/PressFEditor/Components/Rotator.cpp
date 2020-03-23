#include "Rotator.h"

void Rotator::Update(const ImGuiIO& io)
{
	transform->Rotate(speed * io.DeltaTime);
	//PF_INFO("Rotation : {0}", transform->GetRotation().z);
}

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

void Rotator::Start()
{
}

void Rotator::ImGui()
{
}
