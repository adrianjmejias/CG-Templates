#include "Light.h"



Light::Light(GameObject & go, Transform & t, LightType ty) : Component(go, t), type(ty) {}

Light::~Light()
{
}

void Light::Bind(iVec3 & countLights, const ShaderProgram & shader) {
	int& myCount = countLights[static_cast<int>(type)];

	std::string name = "LIGHTS[" + std::to_string(myCount) + "]";

	GLCALL(shader.SetUniform(name + ".type", myCount));
	GLCALL(shader.SetUniform(name + ".position", transform.GetPosition()));
	GLCALL(shader.SetUniform(name + ".attenuation", attenuation));
	GLCALL(shader.SetUniform(name + ".innerAngle", glm::radians(innerAngle)));
	GLCALL(shader.SetUniform(name + ".outerAngle", glm::radians(outterAngle)));

	GLCALL(shader.SetUniform(name + ".direction", transform.front));
	GLCALL(shader.SetUniform(name + ".isOn", (bool)enabled));

	GLCALL(shader.SetUniform(name + ".kA", kA));
	GLCALL(shader.SetUniform(name + ".kD", kD));
	GLCALL(shader.SetUniform(name + ".kS", kS));

	myCount++;
}

void Light::HandleEvent(const SDL_Event & e)
{
}

void Light::Update()
{
}

