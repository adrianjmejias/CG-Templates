#include "PressF/pch.h"
#include "PressF/Rendering/Texture.h"



namespace PF
{
	class FrameBuffer : public GPUObject
	{
	public:
		int width, height;
		std::vector<GLenum> attachmentIds;


		FrameBuffer() = default;

		// Inherited via GPUObject
		virtual void Bind() override;
		virtual void UnBind();

		std::vector<Texture> colors;
		std::vector<RenderBuffer> depthBuffers;



		Texture* AddColorAttachment();
		RenderBuffer* AddDepthAttachment();
		//Texture& 

		void Clear();

		void SetSize(int nWidth, int nHeight);

		void Generate();

		void AttachColorBuffers();

		void AttachDepthBuffer();

		void GPUInstantiate();
		bool CheckErrors();
	};
}