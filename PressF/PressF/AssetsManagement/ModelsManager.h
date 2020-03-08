#pragma once 
#include "AssetsManager.h"
#include "../Model.h"


namespace PF{
	class ModelsManager : AssetsManager<Model>
	{
		

	public:
		// Inherited via AssetsManager
		virtual Model& LoadAsset(const std::string&) override;
		// Inherited via AssetsManager
		//virtual bool LoadAsset(const std::string&) override;
	};

}