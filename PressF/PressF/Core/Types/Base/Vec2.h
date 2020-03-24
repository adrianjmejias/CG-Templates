#pragma once 

using Vec2 = glm::vec2;

inline bool ImGuiRender(Vec2& val, const std::string& name = "", float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f)
{
	return ImGui::DragFloat2(name.c_str(), &val.x, v_speed, v_min, v_max, format, power);
}