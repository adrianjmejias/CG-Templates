#include "PressF/pch.h"
#include "Asset.h"
namespace PF
{
	Asset::Asset(const std::string& n) : name(n) {}
	json Asset::Serialize()
	{
		// TODO: insert return statement here
		json j;

		j["name"] = name;
		j["path"] = path;
		j["assetId"] = assetId;
		
		return j;
	}
}