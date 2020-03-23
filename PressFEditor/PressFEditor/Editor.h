

#pragma once
#include <PressF/PressF.h>
#include "Components/Components.h"
#include <iomanip>


namespace PF
{
	static std::vector<GameObject*> GetAsSeparateGameObjects(Model& model);
}

class Editor
{
	PF::Engine engine;
	ImGui::FileBrowser fbrowser;

public: 
	Editor() = default;
	void Init();
	void LoopImGui();
};