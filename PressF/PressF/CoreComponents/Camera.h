#pragma once 
#include "PressF/EntityComponent/Component.h"

namespace PF
{
	class Camera : public Component
	{
	public:
		UInt priority;
		// Inherited via Component
		virtual void Update(const ImGuiIO& io) override;
		virtual void HandleEvent(const SDL_Event& e) override;
		virtual json Serialize() override;

		// Inherited via Component
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		// Inherited via Component
		virtual void Start() override;
	};
}