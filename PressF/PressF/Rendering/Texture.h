#pragma once
#include "PressF/Core/OpenGL.h"
#include "PressF/AssetsManagement/Asset.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



namespace PF
{

	enum class ColorFormat {
		RED = GL_RED,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
	};

	struct Texture : GPUObject, Asset {
		Int width;
		Int height;
		Int nComponents;
		ColorFormat format = ColorFormat::RED;

		static Owns<Texture> Texture::TextureFromFile(const std::string& _path);

		virtual void Bind() override;

		~Texture();

		virtual void ImGui();
	};

}