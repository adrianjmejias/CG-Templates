#pragma once
#include <PressF/PressF.h>
#include <imgui.h>
class Rotator : public PF::Component
{
public:
	Vec3 speed{ 1.f, 1.f ,1.f };

	// Inherited via Component
	virtual void Update(const ImGuiIO& io) override;
	virtual void HandleEvent(const SDL_Event& e) override;

	// Inherited via Component
	virtual json Serialize() override;

	// Inherited via Component
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	// Inherited via Component
	virtual void Start() override;

	// Inherited via Component
	virtual void ImGui() override;
	// Inherited via Component
};