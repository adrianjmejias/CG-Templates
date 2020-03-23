#pragma once
#include "ShaderParameter.h"

namespace PF
{
	//struct ShaderInt : Int, ShaderParameterValue
	//{
	//	ShaderInt(const Int& val) : Int(val) {}

	//	virtual void Bind(const std::string name, const Ref<ShaderProgram>& shader) override
	//	{
	//		shader->SetUniform(name, value);
	//	}

	//	SP_IMGUI_RENDER_VARIABLE()
	//};

	SP_CLASS_VARIABLE(Int);
}