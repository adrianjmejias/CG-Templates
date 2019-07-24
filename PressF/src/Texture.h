#pragma once
#include "types.h"
#include <stb_image.h>
class Texture
{
	Texture();
public:

	unsigned int id = 0;

	static Texture* TextureFromFile(const char *path, const std::string &directory, bool gamma);




	~Texture();
};

