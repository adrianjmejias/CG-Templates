#include "PressF/pch.h"
#include "Renderer.h"
#include "PressF/Core/EngineConfig.h"
#include "PressF/Core/Window.h"
#include "Utility/Quad.h"
//#include "PressF/Core/OpenGL.h"

namespace PF
{
	VertexArrayObject cubevao;
	VertexBufferObject cubevbo;
	void renderCube()
	{
		// initialize (if necessary)
		if (cubevao.id == 0)
		{
			float vertices[] = {
				// back face
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
				 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
				// front face
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
				 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				// left face
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				// right face
				 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
				 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
				// bottom face
				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
				 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				// top face
				-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
				 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
			};
			glGenVertexArrays(1, &cubevao.id);
			glGenBuffers(1, &cubevao.id);
			// fill buffer
			glBindBuffer(GL_ARRAY_BUFFER, cubevbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// link vertex attributes
			glBindVertexArray(cubevao);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		// render Cube
		glBindVertexArray(cubevao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
	
	Owns<Renderer> Renderer::instance = nullptr;

	Renderer* Renderer::GetInstance()
	{
		if (instance.get() == nullptr)
		{
			auto r = new Renderer();
			Window* win{ Window::GetInstance() };
			r->OnResize(win->width, win->height);
			r->RecompileShaders();
			instance.reset(r);
		}
		return instance.get();
	}

	void Renderer::RecompileShaders()
	{
		geometryPass.reset(new ShaderProgram("../assets/shaders/dfgeo.vert", "../assets/shaders/dfgeo.frag"));
		shaderLightingPass.reset(new ShaderProgram("../assets/shaders/dflighting.vert", "../assets/shaders/dflighting.frag"));
		shaderLightBox.reset(new ShaderProgram("../assets/shaders/slb.vert", "../assets/shaders/slb.frag"));
		shaderQuad.reset(new ShaderProgram("../assets/shaders/qShader.vert", "../assets/shaders/qShader.frag"));
		shaderSSAO.reset(new ShaderProgram("../assets/shaders/ssao.vert", "../assets/shaders/ssao.frag"));
	}

	void Renderer::RegisterMesh(MeshRenderer* mesh, RenderMask renderMask)
	{
		UnRegisterMesh(mesh);

		for (int ii = 0; ii < renderMask.size(); ii++)
		{
			if (renderMask[ii] == true)
			{
				objects[ii][mesh->mesh].push_back(mesh);
			}
		}


	}
	void Renderer::UnRegisterMesh(MeshRenderer* mesh)
	{
		for (int ii = 0; ii < PF_RENDER_MASKS_SIZE; ii++)
		{
			auto& objs = objects[ii];

			auto ito = objs.find(mesh->mesh);

			if (ito != objs.end())
			{
				auto mrList = ito->second;

				auto it = std::find(mrList.begin(), mrList.end(), mesh);

				if (it != mrList.end())
				{
					mrList.erase(it);
				}
			}
		}
	}

	void Renderer::RegisterCamera(Camera* cam)
	{
		cameras.push(cam);
	}
	void Renderer::UnRegisterCamera(Camera* cam)
	{
		std::list<Camera*> cams;

		// esto esta horrible, arreglar despues
		while (!cameras.empty())
		{
			Camera* c = cameras.top();

			if (c != cam)
			{
				cams.push_back(c);
			}
		}

		while (!cams.empty())
		{
			cameras.push(cams.front());
			cams.pop_front();
		}
	}

	void Renderer::RegisterParticleSystem(ParticleSystem* ps)
	{
		particlesSystems.push_back(ps);
	}
	void Renderer::UnRegisterParticleSystem(ParticleSystem* ps)
	{
		auto it = std::find(particlesSystems.begin(), particlesSystems.end(), ps);

		if (it != particlesSystems.end())
		{
			std::swap(particlesSystems.end() - 1, it);
			particlesSystems.pop_back();
		}

	}



	void Renderer::RegisterLight(Light* l)
	{
		lights.push_back(l);
	}
	void Renderer::UnRegisterLight(Light* l)
	{
		auto it = std::find(lights.begin(), lights.end(), l);

		if (it != lights.end())
		{
			std::swap(lights.end() - 1, it);
			lights.pop_back();
		}
	}

	void Renderer::BindLigths(ShaderProgram& shader)
	{

		shader.SetUniform("nLights", static_cast<int>(lights.size()));
		for (int ii = 0; ii < lights.size(); ii++)
		{
			auto& l = *lights[ii];
			std::string name = "LIGHTS[" + std::to_string(ii) + "]";
			shader.SetUniform(name + ".position", l.transform->GetPosition());
			shader.SetUniform(name + ".kA", l.kA);
			shader.SetUniform(name + ".kD", l.kD);
			shader.SetUniform(name + ".kS", l.kS);
			shader.SetUniform(name + ".attenuation", l.attenuation);
		}
	}

	void Renderer::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);
		Window* win{ Window::GetInstance() };
		Camera* c = cameras.top();
		c->transform->TryGetClean();
		const Vec3& viewPos = c->transform->GetPosition();
		EngineConfig& ec = *EngineConfig::GetInstance();

		renderingType = ec.useDeferredRendering ? RenderingType::Deferred : RenderingType::Forward;


		if (renderingType == RenderingType::Forward)
		{
			if (ec.useStereoscopic)
			{
				int hWidth = win->width / 2;
				int hHeigth = win->height / 2;
				float ar = hWidth / float(win->height);

				const Mat4& projection = c->GetProjectionMatrix(ar);
				auto [vl, vr] = c->GetViewMatrixStereoscopic(ec.IOD, ec.zDistance);

				glViewport(0, 0, hWidth, win->height);

				RenderNormal(projection, vl, viewPos);

				glViewport(hWidth, 0, hWidth, win->height);

				RenderNormal(projection, vr, viewPos);
			}
			else
			{
				const Mat4& projection = c->GetProjectionMatrix();

				glViewport(0, 0, win->width, win->height);

				const Mat4& view = c->GetViewMatrix();
				RenderNormal(projection, view, viewPos);
			}
		}

		if (renderingType == RenderingType::Deferred)
		{
			glViewport(0, 0, win->width, win->height);

			const Mat4& projection = c->GetProjectionMatrix();
			const Mat4& view = c->GetViewMatrix();

			std::bitset<8> renderStages;
			// Geometry pass

			
			glBindFramebuffer(GL_FRAMEBUFFER, fb);
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				geometryPass->Bind();
				geometryPass->SetUniform("projection", projection);
				geometryPass->SetUniform("view", view);
				//geometryPass->SetUniform("stepSize", ec.stepSize);
				//geometryPass->SetUniform("convSize", ec.convSize);
				//geometryPass->SetUniform("convPivot", ec.convPivot);

				for (int ii = 0; ii < PF_RENDER_MASKS_SIZE; ii++)
				{
					auto& objs = objects[ii];
					for (auto [mesh, mrList] : objs)
					{
						mesh->Bind();
						for (auto mr : mrList)
						{
							mr->mat->BindParametersOnly(geometryPass.get());
							geometryPass->SetUniform("model", mr->transform->GetAccumulated());
							geometryPass->SetUniform("bloom", ec.useBloom && int(mr->renderMask[PF_BLOOM]));

							mesh->Render();
							//mesh->Draw(*geometryPass.get());
						}
					}
				}
			}

			
			glBindFramebuffer(GL_FRAMEBUFFER, ssaofb);
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			shaderSSAO->Bind();
			
			for (unsigned int i = 0; i < ssaoKernel.size(); ++i)
			{
				shaderSSAO->SetUniform("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
			}
			shaderSSAO->SetUniform("projection", projection);


			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, fb.colors[0]);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, fb.colors[1]);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, noiseTex);
			
			Quad::Instance()->Bind();
			Quad::Instance()->Draw();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			



			/*

			// Lighting pass
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			shaderLightingPass->Bind();

			glActiveTexture(GL_TEXTURE0);
			fb.colors[0].Bind();

			glActiveTexture(GL_TEXTURE1);
			fb.colors[1].Bind();

			glActiveTexture(GL_TEXTURE2);
			fb.colors[2].Bind();


			shaderLightingPass->SetUniform("gPosition", 0);
			shaderLightingPass->SetUniform("gNormal", 1);
			shaderLightingPass->SetUniform("gAlbedoSpec", 2);

			shaderLightingPass->SetUniform("nLights", static_cast<int>(lights.size()));
			for (int ii = 0; ii < lights.size(); ii++)
			{
				auto& l = *lights[ii];
				std::string name = "LIGHTS[" + std::to_string(ii) + "]";
				shaderLightingPass->SetUniform(name + ".Position", l.transform->GetPosition());
				shaderLightingPass->SetUniform(name + ".Color", l.kD);

				float constant = l.attenuation.x;
				float linear = l.attenuation.y;
				float quadratic = l.attenuation.z;

				shaderLightingPass->SetUniform(name + ".Linear", linear);
				shaderLightingPass->SetUniform(name + ".Quadratic", quadratic);

				const float maxBrightness = std::fmaxf(std::fmaxf(l.kD.r, l.kD.g), l.kD.b);
				float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
				shaderLightingPass->SetUniform(name + ".Radius", radius);
			}
			
			*/


			shaderQuad->Bind();
			glActiveTexture(GL_TEXTURE0);
			renderTextures[ec.showTex]->Bind();


			//fb.colors[0].Bind();

			//glActiveTexture(GL_TEXTURE1);
			//fb.colors[1].Bind();

			//glActiveTexture(GL_TEXTURE2);
			//fb.colors[2].Bind();

			shaderQuad->SetUniform("img", 0);

			Quad::Instance()->Bind();
			Quad::Instance()->Draw();

			//// 2.5. copy content of geometry's depth buffer to default framebuffer's depth buffer
			//// ----------------------------------------------------------------------------------
			//glBindFramebuffer(GL_READ_FRAMEBUFFER, fb);
			//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
			//// blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
			//// the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
			//// depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
			//glBlitFramebuffer(0, 0, fb.width, fb.height, 0, 0, fb.width, fb.height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);



			/*------------------ */


			//// 3. render lights on top of scene
			//// --------------------------------
			//shaderLightBox->Bind();
			//shaderLightBox->SetUniform("projection", projection);
			//shaderLightBox->SetUniform("view", view);
			//for (unsigned int i = 0; i < lights.size(); i++)
			//{
			//	auto& l = *lights[i];
			//	shaderLightBox->SetUniform("model", l.transform->GetAccumulated());
			//	shaderLightBox->SetUniform("lightColor", l.kD);
			//	renderCube();
			//}
		}


	}
	void Renderer::RenderNormal(const Mat4& projection, const Mat4& view, const Vec3& viewPos)
	{
		RenderParticles(projection, view, viewPos);

		// Renderizo aca
		RenderMeshes(projection, view, viewPos, objects[PF_BLOOM]);

		// Renderizo aca
		RenderMeshes(projection, view, viewPos, objects[PF_NORMAL]);

		// Combino bloom y normal

		// TRANSPARENT SORT HERE

		RenderMeshes(projection, view, viewPos, objects[PF_TRANSPARENT]);
	}
	
	void Renderer::RenderParticles(const Mat4& projection, const Mat4& view, const Vec3& viewPos)
	{
		for (auto ps : particlesSystems)
		{
			//ps->UpdateRender();
			auto it = ps->GetIterator();

			ps->mesh->Bind();
			ps->mat->Bind();

			auto shader = ps->mat->shader;
			const Mat4& parentModel{ ps->transform->GetAccumulated() };

			if (ps->isHairSimulation)
			{
				shader->SetUniform("view", view);
				shader->SetUniform("projection", projection);
				shader->SetUniform("viewPos", viewPos);

				while (auto psp = it.GetNext())
				{
					float t = 0;

					while (t <= 1)
					{
						auto [pos, rot, scale] = psp->GetSimulationDelta(t);
						const Mat4& model{ Transform::GenModel(scale, pos, rot) };

						shader->SetUniform("model", parentModel * model);
						ps->mesh->Render();

						t += ps->dHair;
					}
				}
			}
			else
			{
				while (auto psp = it.GetNext())
				{
					const Mat4& model{ Transform::GenModel(psp->scale, psp->initialPosition, psp->rotation) };
					shader->SetUniform("model", parentModel * model);
					shader->SetUniform("view", view);
					shader->SetUniform("projection", projection);
					ps->mesh->Render();
				}
			}


		}
	}

	void Renderer::RenderMeshes(const Mat4& projection, const Mat4& view, const Vec3& viewPos, std::unordered_map < GPUMesh*, std::list<MeshRenderer*>> objs)
	{
			for (auto [mesh, mrList] : objs)
			{
				mesh->Bind();
				for (auto mr : mrList)
				{
					mr->mat->Bind();

					auto shader = mr->mat->shader;

					if (mr->mat->usesLight)
					{
						BindLigths(*shader);
					}

					shader->SetUniform("viewPos", viewPos);
					shader->SetUniform("model", mr->transform->GetAccumulated());
					shader->SetUniform("view", view);
					shader->SetUniform("projection", projection);

					mesh->Render();
				}
			}

		
	}

	void Renderer::OnResize(int nWidth, int nHeight)
	{

		renderTextures.clear();
		Window* win{ Window::GetInstance() };

		fb.Clear();
		fb.width = nWidth;
		fb.height = nHeight;

		fb.AddColorAttachment();
		fb.AddColorAttachment();
		fb.AddColorAttachment();


		for (int i = 0; i < fb.colors.size()-1; i++)
		{
			renderTextures.push_back(&fb.colors[i]);
			Texture& color =  fb.colors[i];
			color.format = TexColorFormat::RGB;
			color.internalFormat = TexColorFormat::RGB_16F;
			color.texPixelType = TexPixelType::FLOAT;

			color.sInterpolation = TexInterpolationMethod::NEAREST;
			color.tInterpolation = TexInterpolationMethod::NEAREST;

			color.sClamp = TexClampMethod::CLAMP;
			color.tClamp = TexClampMethod::CLAMP;
		}

		fb.AddDepthAttachment();
		{
			renderTextures.push_back(&fb.colors[2]);

			Texture& color = fb.colors[2];

			color.format = TexColorFormat::RGB;
			color.internalFormat = TexColorFormat::RGB;
			color.texPixelType = TexPixelType::UNSIGNED_BYTE;

			color.sInterpolation = TexInterpolationMethod::NEAREST;
			color.tInterpolation = TexInterpolationMethod::NEAREST;

			color.sClamp = TexClampMethod::CLAMP;
			color.tClamp = TexClampMethod::CLAMP;
		}

		

		// ping-pong-framebuffer for blurring
		for (unsigned int i = 0; i < 2; i++)
		{
			auto& ppfb = pingPong[i];
			ppfb.Clear();
			auto& color = *ppfb.AddColorAttachment();
			renderTextures.push_back(&color);

			color.format = TexColorFormat::RGBA;
			color.internalFormat = TexColorFormat::RGBA_16F;
			color.texPixelType = TexPixelType::FLOAT;

			color.sInterpolation = TexInterpolationMethod::LINEAR;
			color.tInterpolation = TexInterpolationMethod::LINEAR;

			color.sClamp = TexClampMethod::CLAMP;
			color.tClamp = TexClampMethod::CLAMP;
			ppfb.SetSize(nWidth, nHeight);
		}
		fb.SetSize(nWidth, nHeight);


		ssaofb.AddColorAttachment();
		{
			renderTextures.push_back(&ssaofb.colors[0]);
			Texture & ssaoColor = ssaofb.colors[0];
			ssaoColor.internalFormat = TexColorFormat::RED;
			ssaoColor.format = TexColorFormat::RGB;
			ssaoColor.texPixelType = TexPixelType::FLOAT;
		}
		ssaofb.SetSize(nWidth, nHeight);


		EngineConfig& ec = *EngineConfig::GetInstance();
		ssaoKernel.clear();
		for (unsigned int i = 0; i < 64; ++i)
		{
			glm::vec3 sample(glm::linearRand(0.f, 1.f) * 2.0 - 1.0, glm::linearRand(0.f, 1.f) * 2.0 - 1.0, glm::linearRand(0.f, 1.f));
			sample = glm::normalize(sample);
			sample *= glm::linearRand(0.f, 1.f);
			float scale = float(i) / 64.0f;

			// scale samples s.t. they're more aligned to center of kernel
			scale = glm::mix(0.1f, 1.0f, scale * scale);
			sample *= scale;
			ssaoKernel.push_back(sample);
		}

		// generate noise texture
		// ----------------------
		ssaoNoise.clear();
		for (unsigned int i = 0; i < 16; i++)
		{
			glm::vec3 noise{ (glm::linearRand(0.f, 1.f) * 2.0 - 1.0, (glm::linearRand(0.f, 1.f) * 2.0 - 1.0, 0.0f)) }; // rotate around z-axis (in tangent space)
			ssaoNoise.push_back(noise);
		}

		noiseTex.internalFormat = TexColorFormat::RGB_32F;
		noiseTex.format = TexColorFormat::RGB;
		noiseTex.texPixelType = TexPixelType::FLOAT;
		noiseTex.sClamp = TexClampMethod::REPEAT;
		noiseTex.tClamp = TexClampMethod::REPEAT;
		noiseTex.sInterpolation = TexInterpolationMethod::NEAREST;
		noiseTex.tInterpolation = TexInterpolationMethod::NEAREST;
		noiseTex.SetSize(4, 4);

   // -----------------------------------------------------
		//glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
		//glGenTextures(1, &ssaoColorBufferBlur);
		//glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
		//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		//	std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}