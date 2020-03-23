#pragma once


using Color = glm::vec3;


inline bool ImGuiRenderColor(Color& val, const std::string& name = "", ImGuiColorEditFlags flags = 0, const float* ref_col = NULL)
{
	return ImGui::ColorEdit3(name.c_str(), &val.x, flags);
}
