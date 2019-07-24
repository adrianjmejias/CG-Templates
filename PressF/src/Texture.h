#pragma once
#include "types.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class Texture
{
	Texture();
public:

	unsigned int id = 0;

	static Texture* TextureFromFile(const char *path, const std::string &directory);


	~Texture();
};

