#include "ModelsManager.h"


namespace PF
{
	Model& ModelsManager::LoadAsset(const std::string& path)
	{
		// TODO: insert return statement here

		if (assets.find(path) != assets.end())
		{

		}


		auto it = assets.emplace(path, std::make_unique<Model>());


		return *it.first->second;
	}
}
