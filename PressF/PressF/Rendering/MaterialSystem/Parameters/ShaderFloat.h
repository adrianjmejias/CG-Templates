#pragma once
#include "ShaderParameter.h"

namespace PF
{
	//struct ShaderFloat : V, ShaderParameterValue
	//{
	//	ShaderVec3(const Vec3& val) : Vec3(val) {}

	//	virtual void Bind(const std::string name, const Ref<ShaderProgram>& shader) override
	//	{
	//		shader->SetUniform(name, *this);
	//	}

	//	IMGUI_RENDER_VARIABLE()
	//};

	SP_CLASS_VARIABLE(Float);
}