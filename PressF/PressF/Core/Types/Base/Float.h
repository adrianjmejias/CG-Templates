#pragma once
#include "BaseDataType.h"

struct Float : BaseDataType<float> {
};

inline void ImGuiRender(Float& val, const std::string& name = "", float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f)
{
	ImGui::DragFloat(name.c_str(), &val.value, v_speed, v_min, v_max, format, power);
}