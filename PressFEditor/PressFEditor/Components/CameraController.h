#pragma once
#include <PressF/PressF.h>



class CameraController : public PF::Component
{
	// Inherited via Component
	virtual void Update(const ImGuiIO& io) override;
	virtual void HandleEvent(const SDL_Event& e) override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void Start() override;
	virtual json Serialize() override;
};