#pragma once
#include "ShaderParameter.h"

namespace PF
{
	struct ShaderColor : Color, ShaderParameterValue
	{
		// Inherited via ShaderParameterValue
		
		ShaderColor(Color c): Color(c)
		{

		}



		SP_BIND_VARIABLE();


		void ShaderColor::ImGui(const std::string& name)
		{
			ImGuiRenderColor(*this, name.c_str());
		}
	};
}