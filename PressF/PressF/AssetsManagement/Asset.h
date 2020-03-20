#pragma once
#include "../pch.h"
#include "Serialization.h"

namespace PF
{
	struct Asset: public Serializable
	{
	public:
		unsigned int assetId = 0;
		std::string name;
		std::string path;

		// Inherited via Serializable
		virtual json Serialize() override;
	};
}	

