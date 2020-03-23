#include "PressF/pch.h"
#include "ShaderParameter.h"

namespace PF
{
	ShaderParameter::ShaderParameter()
	{
	}
	void ShaderParameter::ImGui()
	{
		ImGui::Text(name.c_str()); ImGui::SameLine(); value->ImGui(name.c_str());
	}
	void ShaderParameter::Bind(const Ref<ShaderProgram>& s) {
		value->Bind(name, s);
	}
}