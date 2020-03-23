#include "PressF/pch.h"


namespace PF
{

	template  <typename Key, typename AssetType>
	struct AssetMapBase : public std::map<std::string, Owns<AssetType>>
	{
		std::string name;
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
		virtual void ImGui() = 0;
	};

	template  <typename AssetType>
	struct AssetMap : public AssetMapBase<std::string, AssetType>
	{
		AssetMap(const std::string& n) {
			name = n;
		}

		virtual void ImGui() override
		{
			if (ImGui::CollapsingHeader(name.c_str())) {
				for(auto it = begin(); it != end(); it++)
				{
					it->second->ImGui();
				}
				ImGui::Separator();
			}
		}
	};
}