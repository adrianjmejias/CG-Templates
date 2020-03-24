#include "PressF/pch.h"
#include "EngineConfig.h"

namespace PF
{
	Owns<EngineConfig> EngineConfig::instance = nullptr;
	EngineConfig* EngineConfig::GetInstance()
	{
		if (!instance.get())
		{
			instance.reset(new EngineConfig());
		}


		return instance.get();
	}
	void PF::EngineConfig::ImGui()
	{
		ImGuiRender(showFPS, "Show FPS");

		if (showFPS)
		{
			ImGui::SameLine();
			ImGui::Text(": %f", ImGui::GetIO().Framerate);
		}

		ImGuiRender(useStereoscopic, "Use Stereoscopic");

		if (useStereoscopic)
		{
			ImGuiRender(IOD, "IOD");
			ImGuiRender(zDistance, "Z Distance");
		}


		ImGuiRender(useSSAO, "Use SSAO");
		ImGuiRender(useBloom, "Use Bloom");
		ImGuiRender(useDeferredRendering, "Use Deferred Shading");
	}

}
