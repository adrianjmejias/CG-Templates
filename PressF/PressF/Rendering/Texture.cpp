#include "PressF/pch.h"
#include "Texture.h"
namespace PF
{
	void Texture::Gen()
	{
		if (id)
		{
			glDeleteTextures(1, &id);
		}

		glGenTextures(1, &id);
	}
	void Texture::SetInterpolationMethod(TexInterpolationMethod s, TexInterpolationMethod t)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<int>(s));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<int>(t));
	}
	void Texture::SetClampMethod(TexClampMethod s, TexClampMethod t)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(s));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(t));
	}
	Owns<Texture> Texture::TextureFromFile(const std::string& _path)
	{
		Owns<Texture> tex{ new Texture() };
		tex->path = _path;

		glGenTextures(1, &tex->id);

		unsigned char* data = stbi_load(tex->path.c_str(), &tex->width.value, &tex->height.value, &tex->nComponents.value, 0);
		if (data)
		{
			switch (tex->nComponents)
			{
			case 1:
				tex->format = static_cast<TexColorFormat>(GL_RED);
				break;
			case 3:
				tex->format = static_cast<TexColorFormat>(GL_RGB);
				break;
			case 4:
				tex->format = static_cast<TexColorFormat>(GL_RGBA);
				break;
			default:
				break;
			}

			glBindTexture(GL_TEXTURE_2D, tex->id);
			glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(tex->format), tex->width, tex->height, 0, static_cast<int>(tex->format), GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << tex->path << std::endl;
			stbi_image_free(data);
		}

		return tex;
	}
	void Texture::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void Texture::UnBind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
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
