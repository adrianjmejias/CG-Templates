#include "PressF/pch.h"
#include "PressF/Rendering/Renderer.h"
#include "ParticleSystem.h"

namespace PF{

	static constexpr unsigned int GetNextParticleIndex(UInt ii, UInt maxParticles)
	{
		return  (ii + 1) % maxParticles;
	}


	void ParticleSystem::SetSpawningRate(float _spawningRate)
	{
		*spawningRate = _spawningRate;
		clockEmit.time.max = spawningRate;
	}

	void ParticleSystem::SetMaxParticles(int _maxParticles)
	{
		*maxParticles = _maxParticles;
		particles.reserve(maxParticles);
		for (UInt ii{ 0 }; ii < maxParticles; ii++)
		{
			particles.emplace_back();
		}
	}

	void ParticleSystem::Start()
	{
		*clockEmit.time.min = spawningRate;
		clockEmit.onTimePassed = [&]() { Emit(); };


		SetSpawningRate(spawningRate);
		SetMaxParticles(maxParticles);
	}

	void ParticleSystem::Emit()
	{
		Particle& p = particles[nextToEmmit];
		p.lifetime.max = particleLifespan;
		p.initialPosition = glm::vec4(glm::linearRand(glm::vec2(-1), glm::vec2(1)), 0, 1);
		p.movementAcceleration = movementAcceleration;
		p.initialSpeed = glm::vec3(glm::ballRand(1.f));


		//PF_INFO("emmited new particle {0}", nextToEmmit);

		*nextToEmmit = GetNextParticleIndex(nextToEmmit, maxParticles);
	}



	constexpr void ParticleSystem::CheckExpiredParticles()
	{
		while (firstEmitted != nextToEmmit && !particles[firstEmitted].IsAlive())
		{
			*firstEmitted = GetNextParticleIndex(firstEmitted, maxParticles);
		}
	}

	// Inherited via Component

	void ParticleSystem::Update(const ImGuiIO& io)
	{
		//PF_INFO("Next particle in {0}", clockEmit.time.GetPorcentage());
		//if (isHairSimulation)
		//{

		//}
		//else
		{
			clockEmit.Update(io.DeltaTime);

			for (UInt ii = firstEmitted; ii != nextToEmmit; *ii = GetNextParticleIndex(ii, maxParticles))
			{
				//PF_INFO("Update particle { 0}", ii);
				particles[ii].UpdateTime(io.DeltaTime);
				//PF_INFO("End Update particle { 0}", ii);
			}
		
			CheckExpiredParticles();
		}
	}

	json ParticleSystem::Serialize()
	{
		return json();
	}

	void ParticleSystem::OnEnable()
	{
		Renderer::GetInstance()->RegisterParticleSystem(this);
	}

	void ParticleSystem::OnDisable()
	{
		Renderer::GetInstance()->UnRegisterParticleSystem(this);
	}

	void ParticleSystem::ImGui()
	{
		auto it = GetIterator();
		ImGuiRender(isHairSimulation, "Is Hair Simulation");

		if (isHairSimulation)
		{
			ImGuiRender(dHair, "Delta Hair", 0.01f, 0.01f, 20.f);

		}
		else
		{
			if (ImGuiRender(maxParticles, "Max particles"))
			{
				SetMaxParticles(maxParticles);
			}

			if (ImGuiRender(spawningRate, "Spawning Rate"))
			{
				SetSpawningRate(spawningRate);
			}

			ImGuiRender(particleLifespan, "Particle Lifespan", 0.2f, 0.2f, 0.2f);
		}

		
		ImGuiRender(initialPosition, "Initial Position");
		if (ImGuiRender(movementAcceleration, "Movement Acceleration", 0.1f))
		{
			while (auto p = it.GetNext())
			{
				p->movementAcceleration = movementAcceleration;
			}
		}
		if (ImGuiRender(initialSpeed, "InitialSpeed", 0.1f))
		{
			{
				while (auto p = it.GetNext())
				{
					p->initialSpeed = initialSpeed;
				}
			}
		}
	}
	ParticleIterator ParticleSystem::GetIterator()
	{
		return ParticleIterator(*this);
	}


	ParticleIterator::ParticleIterator(ParticleSystem& p) : ps(p) { Reset(); }
	void ParticleIterator::Reset()
	{
		act = ps.firstEmitted;
	}
	Particle* ParticleIterator::GetNext()
	{
		unsigned int a = GetNextParticleIndex(act, ps.maxParticles);

		if (a == ps.nextToEmmit)
		{
			return nullptr;
		}
		*act = a;

		return &ps.particles[act];

	}
}