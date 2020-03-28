#include "PressF/pch.h"
#include "Material.h"

namespace PF
{

	void Material::Bind()
	{
		//ShaderParameter* p = new ShaderInt();

		shader->Bind();
		for (auto& sp : parameters)
		{
			sp->Bind(shader);
		}
	}

	void Material::BindParametersOnly(Ref<ShaderProgram> s)
	{
		if (!s)
		{
			s = shader;
		}
		//ShaderParameter* p = new ShaderInt();
		for (auto& sp : parameters)
		{
			sp->Bind(s);
		}
	}
	void Material::BindTexturesOnly(Ref<ShaderProgram> s)
	{
		if (!s)
		{
			s = shader;
		}
		for (int ii = 0; ii < texturesId.size(); ii++)
		{
			glActiveTexture(GL_TEXTURE0 + ii);
			textures[ii]->Bind();
			s->SetUniform(texturesId[ii]->name, ii);
		}
	}
	void Material::AddTexture(const std::string& name, Texture& tex)
	{
		textures.push_back(&tex);
		texturesId.emplace_back(new ShaderParameter(name, new ShaderInt(int(tex.id))));
	}

	void Material::ImGui()
	{
		Asset::ImGui();
		for (auto& sp : parameters)
		{
			sp->ImGui();
		}
	}

}