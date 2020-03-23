#pragma once
#include "BaseDataType.h"

struct UInt : BaseDataType<unsigned int> {
};

inline void ImGuiRender(UInt& val, const std::string& name = "", float v_speed = 1.0, int v_min = 0, int v_max = 0, const char* format = "%d")
{

	if (v_min < 0)
	{
		v_min = 0;
	}


	ImGui::DragInt(name.c_str(), (int*)(&val.value), v_speed, v_min, v_max, format);
}