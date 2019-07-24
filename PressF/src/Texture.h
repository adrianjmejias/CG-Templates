#pragma once
#include "types.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

enum class ColorFormat {
	RED = GL_RED,
	RGB = GL_RGB,
	RGBA = GL_RGBA,
};

class Texture
{
public:
	Texture(int width, int height, ColorFormat format);
	Texture();

	unsigned int id = 0;
	int width, height, nrComponents;
	ColorFormat format;
	static Texture* TextureFromFile(const char *path, const std::string &directory);
	static Texture* TextureCubemap(std::vector < std::string > paths) {

	}
	void Resize(int width, int height);
	~Texture();
};

