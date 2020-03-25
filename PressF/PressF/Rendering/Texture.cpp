#include "PressF/pch.h"
#include "Texture.h"
namespace PF
{
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
