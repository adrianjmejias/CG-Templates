#include "PressF/pch.h"
#include "ShaderParameters.h"


namespace PF
{
	void ShaderParameter::Bind(const Ref<ShaderProgram>& s) {
		value->Bind(name, s);
	}

}
