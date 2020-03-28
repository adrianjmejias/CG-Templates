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

		if (useBloom)
		{
			ImGuiRender(HDRGamma, "HDR Gamma", 0.01f);
			ImGuiRender(HDRExposure, "HDR Exposure", 0.1);

			ImGuiRender(boolBlurAmmount, "Boom Blur", 0.2f, 0, 30);
			ImGuiRender(bloomThreshold, "Boom Threshold", 0.02f, 0, 1);
			ImGuiRender(stepSize, "Step Size", 0.02f);
			ImGuiRender(convSize, "Conv Size", 1);
			ImGuiRender(convPivot, "Conv Pivot", 1);
		}
		ImGuiRender(useDeferredRendering, "Use Deferred Shading");

		if (useDeferredRendering)
		{
			ImGuiRender(showTex, "Texture Number", 0.2f);
		}
	}

}
