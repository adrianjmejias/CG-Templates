#pragma once 
#include <PressF/PressF.h>

	struct Particle
	{
		RangeFloat lifetime;

		Vec3 position;
		Vec3 velocity;
		void UpdateTime(float deltaTime)
		{
			lifetime.min += deltaTime;

			//PF_INFO("particle lifetime {0},  particle lifetime {1}", lifetime.min, lifetime.GetPorcentage());
		}

		void UpdateSimulation(float deltaTime)
		{
			position += velocity * deltaTime;
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
		Float spawningRate{ 3 };
		std::vector<Particle> particles;
		UInt maxParticles{ 500 };
		UInt firstEmitted{ 0 };
		UInt nextToEmmit = { 0 };
		Clock clockEmit;


		virtual void Start();
		virtual void Emit();

		// Inherited via Component
		virtual void Update(const ImGuiIO& io) override;
		virtual void HandleEvent(const SDL_Event& e) override;
		inline unsigned int GetNextParticleIndex(UInt ii);


		// Inherited via Component
		virtual json Serialize() override;

		// Inherited via Component
		virtual void OnEnable() override;
		virtual void OnDisable() override;
	};
