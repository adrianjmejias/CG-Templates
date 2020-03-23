#pragma once
#include "BaseDataType.h"

struct Int : public BaseDataType<int> {
};


inline void ImGuiRender(Int& val, const std::string& name ="", float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d")
{
	ImGui::DragInt(name.c_str(), &val.value, v_speed, v_min, v_max, format);
}