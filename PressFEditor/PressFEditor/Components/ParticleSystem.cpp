#include "ParticleSystem.h"

void ParticleSystem::Start()
{
	clockEmit.time.min = 0;
	clockEmit.time.max = spawningRate;
	clockEmit.onTimePassed = [&]() { Emit(); };


	particles.reserve(maxParticles);
	for (UInt ii = 0; ii < maxParticles; ii++)
	{
		particles.emplace_back();
	}
}

void ParticleSystem::Emit()
{
	particles[nextToEmmit] = Particle();
	//PF_INFO("emmited new particle {0}", nextToEmmit);

	nextToEmmit = GetNextParticleIndex(nextToEmmit);
}

// Inherited via Component

void ParticleSystem::Update(const ImGuiIO& io)
{
	//PF_INFO("Next particle in {0}", clockEmit.time.GetPorcentage());
	clockEmit.Update(io.DeltaTime);


	for (UInt ii = firstEmitted; ii != nextToEmmit; ii = GetNextParticleIndex(ii))
	{
		//PF_INFO("Update particle { 0}", ii);
		particles[ii].UpdateTime(io.DeltaTime);
		//PF_INFO("End Update particle { 0}", ii);
	}

	while (firstEmitted != nextToEmmit && !particles[firstEmitted].IsAlive()) {
		firstEmitted = GetNextParticleIndex(firstEmitted);
	}

	for (UInt ii = firstEmitted; ii != nextToEmmit; ii = GetNextParticleIndex(ii))
	{
		//PF_INFO("Update particle { 0}", ii);
		particles[ii].UpdateSimulation(io.DeltaTime);
		//PF_INFO("End Update particle { 0}", ii);
	}

	for (UInt ii = firstEmitted; ii != nextToEmmit; ii = GetNextParticleIndex(ii))
	{
		//PF_INFO("Update particle { 0}", ii);
		particles[ii].UpdateRender();
		//PF_INFO("End Update particle { 0}", ii);
	}


/*	
	if (nextToEmmit == 11)
	{
		__debugbreak();
	}
	*/
}

void ParticleSystem::HandleEvent(const SDL_Event& e)
{
}

inline UInt ParticleSystem::GetNextParticleIndex(UInt ii)
{
	return  (ii + 1) % maxParticles;
}

json ParticleSystem::Serialize()
{




	return json();
}

void ParticleSystem::OnEnable()
{
}

void ParticleSystem::OnDisable()
{
}
