#pragma once 
#include "../Component.h"

namespace PF
{
	class Camera : public Component
	{
		// Inherited via Component
		virtual void Update(const ImGuiIO& io) override;
		virtual void HandleEvent(const SDL_Event& e) override;
		virtual json Serialize() override;

		// Inherited via Component
		virtual void OnEnable() override;
		virtual void OnDisable() override;
	};
}