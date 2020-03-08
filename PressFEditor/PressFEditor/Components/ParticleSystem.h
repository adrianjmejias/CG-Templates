#pragma once 
#include <PressF/PressF.h>

	struct Particle
	{
		RangeFloat lifetime;
		void UpdateTime(float deltaTime)
		{
			lifetime.min += deltaTime;

			PF_INFO("particle lifetime { 0},  particle lifetime %{1}", lifetime.min, lifetime.GetPorcentage());
		}

		void UpdateSimulation()
		{

		}

		void UpdateRender()
		{

		}

		bool IsAlive()
		{
			return lifetime.max >= lifetime.min;
		}
	};

	struct ParticleSystemConfig
	{
		Float spawningRate;
	};



	//template <typename ParticleConfigType, typename ParticleType>
	class ParticleSystem : public PF::Component
	{
	public:
		Float spawningRate = 3;
		std::vector<Particle> particles;
		UInt maxParticles = 500;
		UInt firstEmitted = 0;
		UInt nextToEmmit = 0;
		Clock clockEmit;
		virtual void Start()
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

		virtual void Emit()
		{
			particles[nextToEmmit] = Particle();
			PF_INFO("emmited new particle {0}", nextToEmmit);

			nextToEmmit = GetNextParticleIndex(nextToEmmit);
		}

		// Inherited via Component
		virtual void Update(const ImGuiIO& io) override
		{
			PF_INFO("Next particle in {0}", clockEmit.time.GetPorcentage());
			clockEmit.Update(io.DeltaTime);


			for (UInt ii = firstEmitted; ii != nextToEmmit; ii = GetNextParticleIndex(ii))
			{
				PF_INFO("Update particle { 0}", ii);
				particles[ii].UpdateTime(io.DeltaTime);
				PF_INFO("End Update particle { 0}", ii);
			}

			while (firstEmitted != nextToEmmit && !particles[firstEmitted].IsAlive()) {
				firstEmitted = GetNextParticleIndex(firstEmitted);
			}

			for (UInt ii = firstEmitted; ii != nextToEmmit; ii = GetNextParticleIndex(ii))
			{
				PF_INFO("Update particle { 0}", ii);
				particles[ii].UpdateSimulation();
				PF_INFO("End Update particle { 0}", ii);
			}


			if (nextToEmmit == 11)
			{
				__debugbreak();
			}
		}

		virtual void HandleEvent(const SDL_Event& e) override
		{
		}

		inline UInt GetNextParticleIndex(UInt ii)
		{
			return  (ii + 1) % maxParticles;
		}

	private:
	};
