#pragma once 
#include "PressF/pch.h"
#include "PressF/EntityComponent/Component.h"
//#include "PressF/Rendering/Renderer.h"


namespace PF{

	struct Particle
	{
		Vec3 initialPosition{0};
		Vec3 rotation{0};
		Vec3 scale{1};
		Vec3 initialSpeed{0,0,1};
		Vec3 movementAcceleration{ 0,0.0f,0 };
		RangeFloat lifetime;

		void UpdateTime(float deltaTime)
		{
			lifetime.min += deltaTime;
		}

		void UpdateSimulation(float deltaTime)
		{
			//position += velocity * deltaTime;
		}

		std::tuple<Vec3, Vec3, Vec3> GetSimulationDelta(float t)
		{

			Float tScaled = lifetime.GetInterpolation(t);
			return {
				// Position
				( 
					initialSpeed +
					(movementAcceleration * tScaled.value /2.f)
				) * tScaled.value + 
				initialPosition
				,

				// Rotation
				rotation,

				// Scale
				scale
			};
		}

		constexpr bool IsAlive()
		{
			return lifetime.max >= lifetime.min;
		}
	};

	class ParticleIterator;
	class ParticleSystem : public Component
	{
		friend class ParticleIterator;
		UInt firstEmitted{ 0 };
		UInt nextToEmmit = { 0 };
		Clock clockEmit;
		std::vector<Particle> particles;
		Float particleLifespan{30.f};

	public:

		void ParticleSystem::SetGPUMesh(GPUMesh* m)
		{
			mesh = m;
			mat = m->mat;
		}
		// ImRender as values
		Float spawningRate{ 3 };
		UInt maxParticles{ 500 };
		Bool isHairSimulation{ true };


		Vec3 initialPosition{ 0 };
		Vec3 rotation{ 0 };
		Vec3 scale{ 1 };



		Vec3 initialSpeed{ 0,0,1 };
		Vec3 movementAcceleration{ 0,0.0f,0 };



		Float dHair{ 0.10f };
		// ImRender as references
		GPUMesh* mesh;
		Material* mat;

		void SetSpawningRate(float _spawningRate);
		void SetMaxParticles(int _maxParticles);

		virtual void Start() override;
		virtual void Emit();

		constexpr void CheckExpiredParticles();

		// Inherited via Component
		virtual void Update(const ImGuiIO& io) override;

		// Inherited via Component
		virtual json Serialize() override;

		// Inherited via Component
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		// Inherited via Component
		virtual void ImGui() override;
		ParticleIterator GetIterator();
	};

	class ParticleIterator
	{
		ParticleSystem& ps;
		UInt act;

	public:
		ParticleIterator(ParticleSystem& p);
		void Reset();
		Particle* GetNext();
	};
}