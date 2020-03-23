#pragma once
#include "BaseDataType.h"

struct Bool : BaseDataType<bool> {
};

inline bool ImGuiRender(Bool& val, const std::string& name = "")
{
	return ImGui::Checkbox(name.c_str(), &val.value);
}