#include "PressF/pch.h"
#include "Light.h"
#include "PressF/Rendering/Renderer.h"

namespace PF
{
	void Light::Bind(int& countLights, const ShaderProgram& shader) {

		//std::string name = "LIGHTS[" + std::to_string(countLights) + "]";

		//GLCALL(shader.SetUniform(name + ".type", static_cast<int>(type)));
		//GLCALL(shader.SetUniform(name + ".position", transform->GetPosition()));
		//GLCALL(shader.SetUniform(name + ".attenuation", attenuation));
		//GLCALL(shader.SetUniform(name + ".innerAngle", glm::radians(innerAngle)));
		//GLCALL(shader.SetUniform(name + ".outerAngle", glm::radians(outterAngle)));

		////GLCALL(shader.SetUniform(name + ".direction", transform->front));
		//GLCALL(shader.SetUniform(name + ".isOn", (bool)enabled));

		//GLCALL(shader.SetUniform(name + ".kA", kA));
		//GLCALL(shader.SetUniform(name + ".kD", kD));
		//GLCALL(shader.SetUniform(name + ".kS", kS));
	}

	void Light::Update(const ImGuiIO& io)
	{
	}

	void Light::OnEnable()
	{
		Renderer::GetInstance()->RegisterLight(this);
	}

	void Light::OnDisable()
	{
		Renderer::GetInstance()->UnRegisterLight(this);

	}

	void Light::Start()
	{
	}

	void Light::ImGui()
	{
		if (ImGui::TreeNode("Light"))
		{
			ImGuiRender(kA, "kA", 0.1f, 0.f, 30.f);
			ImGuiRender(kD, "kD", 0.1f, 0.f, 30.f);
			ImGuiRender(kS, "kS", 0.1f, 0.f, 30.f);
			ImGui::TreePop();
		}
	}

	json Light::Serialize()
	{
		return json();
	}

	void Light::Update()
	{
	}

}