#pragma once
#include "../pch.h"
#include <map>
#include <string>
#include <memory>



namespace PF
{

	class Asset
	{

	};



	template <typename AssetType>
	class AssetsManager
	{
	protected:
		std::map<std::string, uptr<AssetType>> assets;

	public:

		virtual AssetType& LoadAsset(const std::string&) = 0;
	private:
	};
}
