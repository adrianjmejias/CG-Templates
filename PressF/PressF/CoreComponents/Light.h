#pragma once
//#include "core.h"
#include "PressF/pch.h"
#include "PressF/EntityComponent/Component.h"

namespace PF
{


	enum class LightType {
		POINT,
		DIRECTIONAL,
		SPOTLIGHT
	};

	class ShaderProgram;

	class Light : public Component
	{
	public:
		unsigned int idShadow = 0;
		LightType type = LightType::POINT;
		Color kShadow{ 0,0,0 };

		Color kA{ 1,1,1 };
		Color kD{ 0.5,0.5,0.5 };
		Color kS{ 0.5,0.5,0.5 };

		Color kE{ 0,0,0 };

		Color shadowColor{ 0,0,0 };
		Vec3 attenuation{ 0,0,0 };
		Float innerAngle{ 15.f };
		Float outterAngle{ 20.f };


		virtual void Update();
		void Bind(int& countLights, const ShaderProgram& shader);

		// Inherited via Component
		virtual void Update(const ImGuiIO& io) override;

		virtual void OnEnable() override;

		virtual void OnDisable() override;

		virtual void Start() override;

		virtual void ImGui() override;

		virtual json Serialize() override;
	};
}