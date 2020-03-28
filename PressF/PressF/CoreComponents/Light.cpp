#include "PressF/pch.h"
#include "Light.h"
#include "PressF/Rendering/Renderer.h"

namespace PF
{
	void Light::Bind(int& countLights, const ShaderProgram& shader) {


		//GLCALL(shader.SetUniform(name + ".type", static_cast<int>(type)));
		//GLCALL(shader.SetUniform(name + ".attenuation", attenuation));
		//GLCALL(shader.SetUniform(name + ".innerAngle", glm::radians(innerAngle)));
		//GLCALL(shader.SetUniform(name + ".outerAngle", glm::radians(outterAngle)));

		////GLCALL(shader.SetUniform(name + ".direction", transform->front));
		//GLCALL(shader.SetUniform(name + ".isOn", (bool)enabled));
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
			ImGuiRender(attenuation, "Light Attenuation", 0.0002f);
			ImGuiRenderColor(kA, "kA");
			ImGuiRenderColor(kD, "kD");
			ImGuiRenderColor(kS, "kS");
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