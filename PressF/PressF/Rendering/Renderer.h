#pragma once
#include "../pch.h"

#include "PressF/CoreComponents/Camera.h"
#include "PressF/CoreComponents/MeshRenderer.h"
#include "PressF/CoreComponents/ParticleSystem.h"
#include "PressF/CoreComponents/Light.h"
#include "PressF/Rendering/FrameBuffer.h"

namespace PF
{
	enum class RenderingType
	{
		Deferred, 
		Forward
	};
	static std::function<bool(Camera*,Camera*)> cameraComparer = [](Camera* l, Camera* r) -> bool { return *(l->priority) < *(r->priority); };
	struct Renderer
	{
		RenderingType renderingType{RenderingType::Deferred};

		Owns<ShaderProgram> geometryPass;
		Owns<ShaderProgram> shaderLightingPass;
		Owns<ShaderProgram> shaderLightBox;
		Owns<ShaderProgram> shaderQuad;
		Owns<ShaderProgram> shaderSSAO;
		Owns<ShaderProgram> shaderBlur;
		Owns<ShaderProgram> shaderFinal;
		Texture noiseTex;

		FrameBuffer fb;
		FrameBuffer fbFinal;
		FrameBuffer fbBackFaces;
		std::array<FrameBuffer, 2> pingPong;
		FrameBuffer ssaofb;

		std::vector<glm::vec3> ssaoKernel;
		std::vector<glm::vec3> ssaoNoise;

		static Owns<Renderer> instance;
		std::vector<Texture*> renderTextures;
		std::vector<Texture*> finalRenderTextures;

		std::array<std::unordered_map<GPUMesh*, std::list<MeshRenderer*>>, PF_RENDER_MASKS_SIZE> objects;
		std::vector<ParticleSystem*> particlesSystems;
		std::vector<Light*> lights;
		std::priority_queue < Camera*, std::vector<Camera*>, decltype(cameraComparer)> cameras{ cameraComparer };
		static Renderer* GetInstance();


		void RecompileShaders();

		void RegisterMesh(MeshRenderer* mesh, RenderMask renderMask);
		void UnRegisterMesh(MeshRenderer* mesh);


		void RegisterCamera(Camera* cam);
		void UnRegisterCamera(Camera* cam);


		void RegisterParticleSystem(ParticleSystem* ps);
		void UnRegisterParticleSystem(ParticleSystem* ps);

		void RegisterLight(Light* l);
		void UnRegisterLight(Light* l);

		void Render();

		void RenderNormal(const Mat4& projection, const Mat4& view, const Vec3& viewPos);

		void BindLigths(ShaderProgram &shader);

		void RenderParticles(const Mat4& projection, const Mat4& view, const Vec3& viewPos);

		void RenderMeshesDeferred(const Mat4& projection, const Mat4& view, const Vec3& viewPos, std::unordered_map<GPUMesh*, std::list<MeshRenderer*>> objs);

		void RenderMeshes(const Mat4& projection, const Mat4& view, const Vec3& viewPos, std::unordered_map < GPUMesh*, std::list<MeshRenderer*>> objs);


		void OnResize(int nWidth, int nHeight);
	};
}