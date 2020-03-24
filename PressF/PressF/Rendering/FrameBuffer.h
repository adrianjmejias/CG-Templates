#include "PressF/pch.h"
#include "PressF/Rendering/Texture.h"



namespace PF
{
	class FrameBuffer : public GPUObject
	{
	public:
		Owns<RenderBuffer> rbo;
		Owns<Texture> pos;
		Owns<Texture> normal;
		Owns<Texture> color;
		int width, heigth;
		FrameBuffer(int width, int height);
		// Inherited via GPUObject
		virtual void Bind() override;
		virtual void UnBind();
	};
}