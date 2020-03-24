#pragma once
#include "PressF/Core/OpenGL.h"
#include "PressF/AssetsManagement/Asset.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



namespace PF
{

	enum class TexPixelType
	{
		FLOAT = GL_FLOAT,
		UNSIGNED_BYTE  = GL_UNSIGNED_BYTE
	};

	enum class TexColorFormat {
		RED = GL_RED,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		RGB16F = GL_RGB16F,
	};

	enum class TexInterpolationMethod
	{
		LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
		LINEAR = GL_LINEAR,
		NEAREST = GL_NEAREST
	};

	enum class TexClampMethod
	{
		REPEAT = GL_REPEAT,
	};

	enum class TexType
	{
		Texture2D = GL_TEXTURE_2D
	};

	struct Texture : GPUObject, Asset {
		Int width;
		Int height;
		Int nComponents;
		TexColorFormat format = TexColorFormat::RED;
		TexColorFormat internalFormat{TexColorFormat::RGB16F};
		TexType texType;
		Texture() = default;

		Texture(TexColorFormat f, TexColorFormat inf, TexPixelType pType,int w, int h, unsigned char* data = nullptr)
		{
			*width = w;
			*height = h;
			internalFormat = inf;
			format = f;
			Gen();
			Reserve(data);
			SetInterpolationMethod(TexInterpolationMethod::LINEAR, TexInterpolationMethod::LINEAR);
			SetClampMethod(TexClampMethod::REPEAT, TexClampMethod::REPEAT);
		}

		void Reserve(unsigned char* data = nullptr)
		{
			glTexImage2D(static_cast<int>(texType), 0, static_cast<int>(format), width, height, 0, static_cast<int>(format), GL_UNSIGNED_BYTE, data);
		}
		void Gen();
		void SetInterpolationMethod(TexInterpolationMethod s, TexInterpolationMethod t);
		void SetClampMethod(TexClampMethod s, TexClampMethod t);
		static Owns<Texture> Texture::TextureFromFile(const std::string& _path);

		virtual void Bind() override;
		virtual void UnBind();
		~Texture();

		virtual void ImGui();
	};

}