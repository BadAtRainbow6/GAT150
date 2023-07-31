#pragma once
#include "Core/Core.h"
#include "Actor.h"

namespace kiko
{
	struct EmitterData
	{
		// emission
		float spawnRate = 0;
		float spawnRateTimer = 0;
		bool burst = false;
		size_t burstCount = 0;
		// particle parameters
		float lifetimeMin = 0;
		float lifetimeMax = 0;
		float speedMin = 0;
		float speedMax = 0;
		float damping = 0;
		float angle = 0;
		float angleRange = 0;
		kiko::Color color;
	};

	class Emitter : public kiko::Actor
	{
	public:
		Emitter() = default;
		Emitter(const kiko::Transform& transform, const EmitterData& data) :
			Actor{ transform },
			m_data{ data }
		{}
		void Update(float dt);
		void Draw(kiko::Renderer& renderer);
	private:
		void Emit();
	private:
		EmitterData m_data;

		// Inherited via Actor
		virtual void OnCollision(Actor* other) override;
	};
}
