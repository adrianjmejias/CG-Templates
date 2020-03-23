#include "PressF/pch.h"
#include "ShaderParameter.h"

namespace PF
{
	ShaderParameter::ShaderParameter()
	{
	}
	void ShaderParameter::ImGui()
	{
		value->ImGui(name.c_str());
	}
	void ShaderParameter::Bind(const Ref<ShaderProgram>& s) {
		value->Bind(name, s);
	}
}