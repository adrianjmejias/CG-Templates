#pragma once


using Color = glm::vec3;


inline void ImGuiRenderColor(Color& val, const std::string& name = "", ImGuiColorEditFlags flags = 0, const float* ref_col = NULL)
{
	ImGui::ColorEdit3(name.c_str(), &val.x, flags);
}
