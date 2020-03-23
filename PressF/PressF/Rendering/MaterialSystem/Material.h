#include "PressF/pch.h"

#pragma once

#include "PressF/ObjLoader/OBJ_Loader.h"
#include "ShaderProgram.h"
#include "Parameters/ShaderParameters.h"
#include "PressF/Rendering/Texture.h"

namespace PF
{
	Vec3 ToVec3(const objl::Vector3& v);
	Vec2 ToVec2(const objl::Vector2& v);
	Vec4 ToVec4(const objl::Vector3& v);


	struct Material : Asset
	{
	public:
		std::vector<Shares<ShaderParameter>> parameters;
		Ref<ShaderProgram> shader;
		
		Material();
		template <typename TT, typename ...Args >
		void AddParameter(const std::string& name, Args&&... params);
		virtual void Bind();

		Material& operator=(objl::Material& m);

		virtual void ImGui();
	};


	template<typename TT, typename ...Args>
	void Material::AddParameter(const std::string& name, Args&& ...params)
	{
		parameters.emplace_back(ShaderParameter::Create<TT>(name, std::forward<Args>(params)...));
	}
}
