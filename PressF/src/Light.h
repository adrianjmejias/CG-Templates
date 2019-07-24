#pragma once
#include "core.h"

enum class LightType {
	POINT,
	DIRECTIONAL,
	SPOTLIGHT
};

class ShaderProgram;

class Light  : public Component
{
public:
	LightType type;
	Color kA{ 1,1,1,1 };
	Color kD{ 0,0,1,1 };
	Color kS{ 1,0,0,1 };
	Color kE{ 0,0,0,1 };
	Color shadowColor{ 0,0,0,1 };
	Vec3 attenuation;
	float innerAngle = 15.f;
	float outterAngle = 20.f;

	Light(GameObject& go, Transform& t, LightType ty);
	~Light();

	virtual void Update();
	void Bind(iVec3& countLights, const ShaderProgram &shader);
	virtual void HandleEvent(const SDL_Event & e) override;

	//void UI() override {
	//	switch (type)
	//	{
	//	case LightType::POINT:
	//		break;
	//	case LightType::DIRECTIONAL:
	//		break;
	//	case LightType::SPOTLIGHT:
	//		attenuation = nke_Vec3(attenuation, "Attenuation", 0, 20);
	//		innerAngle = nk_propertyf(ctx, "innerAngle", 0, innerAngle, outterAngle, 0.01f, 0.005f);
	//		outterAngle = nk_propertyf(ctx, "outterAngle", innerAngle, outterAngle, 180, 0.01f, 0.005f);
	//		break;
	//	default:
	//		break;
	//	}
	//	nke_Color(kA, "kA");
	//	nke_Color(kD, "kD");
	//	nke_Color(kS, "kS");
	//	nke_Color(kE, "kE");
	//}
	//
	//void HandleEvent(const SDL_Event &e) override {}
	//
	//void ShadowPass() {}
};