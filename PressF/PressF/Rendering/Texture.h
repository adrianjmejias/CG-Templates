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
		RGB_16F = GL_RGB16F,
		RGBA_16F = GL_RGBA16F,
		RGB_32F = GL_RGB32F,
		UBYTE = GL_UNSIGNED_BYTE
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
		CLAMP = GL_CLAMP_TO_EDGE,
	};

	enum class TexType
	{
		Texture2D = GL_TEXTURE_2D
	};

	struct TexMaterialRole
	{
		static const std::string Diffuse;
	};

	struct Texture : GPUObject, Asset {
		Int width;
		Int height;
		Int nComponents;
		TexPixelType texPixelType{TexPixelType::FLOAT};
		TexClampMethod sClamp{TexClampMethod::CLAMP}, tClamp{ TexClampMethod::CLAMP };
		TexInterpolationMethod sInterpolation{TexInterpolationMethod::NEAREST}, tInterpolation{ TexInterpolationMethod::NEAREST };
		TexColorFormat format{ TexColorFormat::RED };
		TexColorFormat internalFormat{TexColorFormat::RGBA_16F};
		TexType texType{TexType::Texture2D};
		Bool generateMipMaps{ false };
		Texture() = default;
		Texture(TexColorFormat f, TexColorFormat inf, TexPixelType pType, int w, int h, unsigned char* data = nullptr);


		void Reserve(unsigned char* data = nullptr);
		void Generate();

		static Owns<Texture> Texture::TextureFromFile(const std::string& _path);

		virtual void Bind() override;
		virtual void UnBind();
		~Texture();

		virtual void ImGui();
		void GPUInstantiate(unsigned char* data = nullptr);

		void SetSize(int nWidth, int nHeight);
	};

}