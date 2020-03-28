#pragma once
#include "ShaderParameter.h"


namespace PF
{
	struct ShaderFloat : Float, ShaderParameterValue
	{
		ShaderFloat(const Float& val) : Float(val) {}
		ShaderFloat(const float& val) : Float(val) {}


		virtual void Bind(const std::string name, const Ref<ShaderProgram>& shader) override
		{
			shader->SetUniform(name, *this);
		}

		SP_IMGUI_RENDER_VARIABLE()
	};

	//SP_CLASS_VARIABLE(Float);
}