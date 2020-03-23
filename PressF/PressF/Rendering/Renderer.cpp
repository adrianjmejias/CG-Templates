#include "PressF/pch.h"
#include "Renderer.h"


namespace PF
{
	Owns<Renderer> Renderer::instance = nullptr;

	Renderer* Renderer::GetInstance()
	{
		if (instance.get() == nullptr)
		{
			instance.reset(new Renderer());
		}
		return instance.get();
	}

	void Renderer::RegisterMesh(MeshRenderer* mesh)
	{
		objects[mesh->mesh].push_back(mesh);
	}

	void Renderer::UnRegisterMesh(MeshRenderer* mesh)
	{
		auto mrList = objects[mesh->mesh];
		//std::remove(mrList.begin(), mrList.end(), mesh);

		auto it = std::find(mrList.begin(), mrList.end(), mesh);

		if (it != mrList.end())
		{
			mrList.erase(it);
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
		for (int ii=0; ii < lights.size(); ii++)
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

		Camera *c = cameras.top();
		c->transform->TryGetClean();
		const Mat4& projection = c->GetProjectionMatrix();
		const Mat4& view = c->GetViewMatrix();
		const Vec3& viewPos = c->transform->GetPosition();
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

		for (auto [mesh, mrList] : objects)
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


}