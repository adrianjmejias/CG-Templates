#pragma once
#include "ShaderParameter.h"

namespace PF
{
	struct ShaderVec3 : Vec3, ShaderParameterValue
	{
		ShaderVec3(const Vec3& val) : Vec3(val) {}

		virtual void Bind(const std::string name, const Ref<ShaderProgram>& shader) override
		{
			shader->SetUniform(name, *this);
		}

		SP_IMGUI_RENDER_VARIABLE()
		//SP_BIND_VARIABLE()
		//SP_IMGUI_RENDER_VARIABLE()
	};
}