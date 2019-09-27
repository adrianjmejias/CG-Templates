#include "types.h"

#include "Texture.h"




Texture::Texture(int width, int height, ColorFormat format)
{
	
	this->width = width;
	this->height = height;
	this->format = format;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, static_cast<int>(format), GL_UNSIGNED_BYTE, NULL);

}


Texture::Texture()
{
}

Texture * Texture::TextureFromFile(const char * path, const std::string & directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	Texture* tex = new Texture();
	glGenTextures(1, &tex->id);

	unsigned char *data = stbi_load(filename.c_str(), &tex->width, &tex->height, &tex->nrComponents, 0);
	if (data)
	{
		switch (tex->nrComponents)
		{
		case 1:
			tex->format = static_cast<ColorFormat>(GL_RED);
			break;
		case 3:
			tex->format = static_cast<ColorFormat>(GL_RGB);
			break;
		case 4:
			tex->format = static_cast<ColorFormat>(GL_RGBA);
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
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return tex;
}

inline void Texture::Resize(int width, int height) {
	glTexImage2D(id, 0, GL_RGB, width, height, 0, static_cast<int>(format), GL_FLOAT, NULL);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id) ;
	std::cout << "Bye Texture"<< std::endl;
}
