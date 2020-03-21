#include "PressF/pch.h"


namespace PF
{

	template  <typename Key, typename AssetType>
	struct AssetMapBase : public std::map<std::string, Owns<AssetType>>
	{
		bool HasAsset(const std::string& path)
		{
			return find(path) != end();
		}
		AssetType* GetAsset(const std::string& path)
		{
			std::map<std::string, Owns<AssetType>>::iterator it = find(path);
			if (it != end())
			{
				return operator[path].get();
			}

			return nullptr;
		}
	};

	template  <typename AssetType>
	struct AssetMap : public AssetMapBase<std::string, AssetType>
	{
	};
}