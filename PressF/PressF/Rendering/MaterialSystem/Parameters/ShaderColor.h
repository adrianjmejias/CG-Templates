#pragma once
#include "ShaderParameter.h"

namespace PF
{
	struct ShaderColor : Color, ShaderParameterValue
	{
		// Inherited via ShaderParameterValue
		
		SP_DEFAULT_CONSTRUCTOR(Color)

		SP_BIND_VARIABLE();


		void ShaderColor::ImGui(const std::string& name)
		{
			ImGuiRenderColor(*this, name.c_str());
		}
	};
}