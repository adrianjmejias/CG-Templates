#include "PressF/pch.h"
#include "Texture.h"
namespace PF
{

	const std::string TexMaterialRole::Diffuse = "texture_diffuse";

	Texture::Texture(TexColorFormat f, TexColorFormat inf, TexPixelType pType, int w, int h, unsigned char* data)
	{
		*width = w;
		*height = h;
		internalFormat = inf;
		format = f;
		Generate();
		Reserve(data);
		//SetInterpolationMethod(TexInterpolationMethod::LINEAR, TexInterpolationMethod::LINEAR);
		//SetClampMethod(TexClampMethod::REPEAT, TexClampMethod::REPEAT);
	}

	void Texture::Reserve(unsigned char* data)
	{
		glTexImage2D(static_cast<int>(texType), 0, static_cast<int>(format), width, height, 0, static_cast<int>(format), GL_UNSIGNED_BYTE, data);
	}

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
		

		unsigned char* data;

		/*
		unsigned int& textureID = tex->id;
		glGenTextures(1, &textureID);
		
		int width, height, nrComponents;
		data = stbi_load(_path.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << _path << std::endl;
			stbi_image_free(data);
		}
		glDeleteTextures(1, &tex->id);
		*/


		tex->path = _path;
		data = stbi_load(tex->path.c_str(), &tex->width.value, &tex->height.value, &tex->nComponents.value, 0);
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
			tex->format = tex->internalFormat;
			tex->texPixelType = TexPixelType::UNSIGNED_BYTE;
			
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
		glBindTexture(static_cast<unsigned int>(texType), id);
	}

	void Texture::UnBind()
	{
		glBindTexture(static_cast<unsigned int>(texType), 0);
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
	void Texture::GPUInstantiate(unsigned char* data)
	{
		Generate();
		Bind();
		glTexImage2D(static_cast<unsigned int>(texType), 0, static_cast<unsigned int>(internalFormat), width, height, 0, static_cast<unsigned int>(format), static_cast<unsigned int>(texPixelType), data);
		if (generateMipMaps)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		glTexParameteri(static_cast<unsigned int>(texType), GL_TEXTURE_WRAP_S, static_cast<unsigned int>(sClamp));
		glTexParameteri(static_cast<unsigned int>(texType), GL_TEXTURE_WRAP_T, static_cast<unsigned int>(tClamp));
		glTexParameteri(static_cast<unsigned int>(texType), GL_TEXTURE_MIN_FILTER, static_cast<unsigned int>(sInterpolation));
		glTexParameteri(static_cast<unsigned int>(texType), GL_TEXTURE_MAG_FILTER, static_cast<unsigned int>(tInterpolation));

	}
	void Texture::SetSize(int nWidth, int nHeight)
	{
		*width = nWidth;
		*height = nHeight;
		GPUInstantiate();
	}
}
