#include "PressF/pch.h"
#include "Texture.h"
namespace PF
{

	const std::string TexMaterialRole::Diffuse = "texture_diffuse";

	void Texture::Generate()
	{
		if (id)
		{
			glDeleteTextures(1, &id);
		}

		glGenTextures(1, &id);
	}
	Owns<Texture> Texture::TextureFromFile(const std::string& _path)
	{
		Owns<Texture> tex{ new Texture() };
		tex->path = _path;

		unsigned char* data = stbi_load(tex->path.c_str(), &tex->width.value, &tex->height.value, &tex->nComponents.value, 0);
		if (data)
		{
			switch (tex->nComponents)
			{
			case 1:
				tex->internalFormat = static_cast<TexColorFormat>(GL_RED);
				break;

			case 3:
				tex->internalFormat = static_cast<TexColorFormat>(GL_RGB);
				break;
			case 4:
				tex->internalFormat = static_cast<TexColorFormat>(GL_RGBA);
				break;
			default:
				break;
			}

			tex->texType = TexType::Texture2D;
			tex->sClamp = TexClampMethod::REPEAT;
			tex->tClamp = TexClampMethod::REPEAT;
			tex->sInterpolation = TexInterpolationMethod::LINEAR_MIPMAP_LINEAR;
			tex->tInterpolation = TexInterpolationMethod::LINEAR;
			*tex->generateMipMaps = true;
			tex->format = TexColorFormat::UBYTE;
			
			tex->GPUInstantiate(data);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << tex->path << std::endl;
			stbi_image_free(data);
			return nullptr;
		}

		return tex;
	}
	void Texture::Bind()
	{
		glBindTexture(static_cast<int>(texType), id);
	}

	void Texture::UnBind()
	{
		glBindTexture(static_cast<int>(texType), 0);
	}

	Texture::~Texture()
	{
		if (id)
		{
			glDeleteTextures(1, &id);
		}
	}
	void Texture::ImGui()
	{
		Asset::ImGui();
	}
}
