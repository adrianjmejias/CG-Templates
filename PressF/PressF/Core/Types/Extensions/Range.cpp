#include "PressF/pch.h"

void ImGuiRender(RangeInt value, const char* label, int v[2], float v_speed, int v_min, int v_max, const char* format)
{
	ImGui::DragInt2(label, &value.min.value, v_speed, v_min, v_max, format);
}
