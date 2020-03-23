#pragma once

using Vec3 = glm::vec3;



inline void ImGuiRender(Vec3& val, const std::string& name = "")
{
	ImGui::DragFloat3(name.c_str(), &val.x);
}