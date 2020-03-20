#include "ShaderParameters.h"


namespace PF
{
	void ShaderParameter::Bind(const Shares<ShaderProgram>& s) {
		value->Bind(name, s);
	}

}
