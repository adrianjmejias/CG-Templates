#include "PressF/pch.h"

#pragma once

#include "PressF/ObjLoader/OBJ_Loader.h"
#include "ShaderProgram.h"
#include "Parameters/ShaderParameters.h"
#include "PressF/Rendering/Texture.h"

namespace PF
{

	struct Material : Asset
	{
	public:
		std::vector<Shares<ShaderParameter>> parameters;
		std::vector <Ref<Texture>> textures;
		std::vector<Shares<ShaderParameter>> texturesId;
		Ref<ShaderProgram> shader;
		bool usesLight = true;

		Material& operator=(objl::Material& m);

		virtual void Bind();
		void BindParametersOnly(Ref<ShaderProgram> s = nullptr);
		void BindTexturesOnly(Ref<ShaderProgram> s = nullptr);


		void AddTexture(const std::string& name, Texture& tex);
		virtual void ImGui();

		template <typename TT, typename ...Args >
		void AddParameter(const std::string& name, Args&&... params);
	};


	template<typename TT, typename ...Args>
	void Material::AddParameter(const std::string& name, Args&& ...params)
	{
		parameters.emplace_back(ShaderParameter::Create<TT>(name, std::forward<Args>(params)...));
	}
}
