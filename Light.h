#pragma once
#include "types.h"


enum class LightType {
	POINT,
	DIRECTIONAL,
	SPOTLIGHT
};

class Light  {
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

	Light();
	~Light();

//	void Bind(iVec3& countLights, const ShaderProgram &shader) {
//#define SEND_DATA_JOINED
//		int& myCount = countLights[static_cast<int>(type)];
//
//#ifdef SEND_DATA_SEPARATED
//		switch (type)
//		{
//		case LightType::DIRECTIONAL:
//			name = "directionals[" + std::to_string(myCount) + "]";
//			break;
//		case LightType::POINT:
//			name = "points[" + std::to_string(myCount) + "]";
//			shader.setVec3(name + ".position", transform.GetPosition());
//			shader.setVec3(name + ".attenuation", attenuation);
//			break;
//		case LightType::SPOTLIGHT:
//			name = "spots[" + std::to_string(myCount) + "]";
//			shader.setVec3(name + ".position", transform.GetPosition());
//			shader.setVec3(name + ".attenuation", attenuation);
//			shader.setFloat(name + ".innerAngle", glm::radians(innerAngle));
//			shader.setFloat(name + ".outerAngle", glm::radians(outterAngle));
//			break;
//		default:
//			__debugbreak(); // this should never happen
//			break;
//		}
//#endif // SEND_DATA_SEPARATED
//
//#ifdef SEND_DATA_JOINED
//		name = "LIGHTS[" + std::to_string(myCount) + "]";
//
//		shader.setInt(name + ".type", static_cast<int>(type));
//		shader.setVec3(name + ".position", transform.GetPosition());
//		shader.setVec3(name + ".attenuation", attenuation);
//		shader.setFloat(name + ".innerAngle", glm::radians(innerAngle));
//		shader.setFloat(name + ".outerAngle", glm::radians(outterAngle));
//#endif // SEND_DATA_JOINED
//
//		shader.setVec3(name + ".direction", transform.front);
//		shader.setBool(name + ".isOn", enabled);
//
//		shader.setVec4(name + ".kA", NKE_COLOR2VEC4(kA));
//		shader.setVec4(name + ".kD", NKE_COLOR2VEC4(kD));
//		shader.setVec4(name + ".kS", NKE_COLOR2VEC4(kS));
//
//		myCount++;
//	}
//	
//	
	//void Update() override {
	//}

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