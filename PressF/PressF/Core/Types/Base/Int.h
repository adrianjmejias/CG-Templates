#pragma once
#include "BaseDataType.h"

struct Int : public BaseDataType<int> {
	Int() = default;
	Int(int v): BaseDataType<int>(v){}
};


inline bool ImGuiRender(BaseDataType<int>& val, const std::string& name ="", float v_speed = 0.20f, int v_min = 0, int v_max = 0, const char* format = "%d")
{
	return ImGui::DragInt(name.c_str(), &val.value, v_speed, v_min, v_max, format);
}