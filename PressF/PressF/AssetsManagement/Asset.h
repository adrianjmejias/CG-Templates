#pragma once
#include "../pch.h"
#include "Serialization.h"

namespace PF
{
	struct Asset: public Serializable
	{
	public:
		unsigned int assetId = 0;
		std::string name= "GameObject";
		std::string path;

		Asset() = default;
		Asset(const std::string& n);
		// Inherited via Serializable
		virtual json Serialize() override;

		virtual void ImGui()
		{
			ImGui::Text(name.c_str());
		}
	};
}	

