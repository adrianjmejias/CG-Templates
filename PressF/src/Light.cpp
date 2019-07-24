#include "Light.h"



Light::~Light()
{
}

void Light::Bind(iVec3 & countLights, const ShaderProgram & shader) {
	int& myCount = countLights[static_cast<int>(type)];

	std::string name = "LIGHTS[" + std::to_string(myCount) + "]";

	shader.SetUniform(name + ".type", static_cast<int>(type));
	shader.SetUniform(name + ".position", transform.GetPosition());
	shader.SetUniform(name + ".attenuation", attenuation);
	shader.SetUniform(name + ".innerAngle", glm::radians(innerAngle));
	shader.SetUniform(name + ".outerAngle", glm::radians(outterAngle));

	shader.SetUniform(name + ".direction", transform.front);
	shader.SetUniform(name + ".isOn", (bool)enabled);

	shader.SetUniform(name + ".kA", kA);
	shader.SetUniform(name + ".kD", kD);
	shader.SetUniform(name + ".kS", kS);

	myCount++;
}

void Light::HandleEvent(const SDL_Event & e)
{
}

void Light::Update()
{
	PF_INFO("UPDATe LIGHT {0}", transform.GetPosition().x);
}

Light::Light(GameObject & go, Transform & t) : Component(go, t) {}

