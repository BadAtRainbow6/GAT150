#pragma once
#include "Core/Core.h"
#include "Renderer/Renderer.h"

struct ParticleData
{
	kiko::vec2 position;
	kiko::vec2 prevPosition;
	kiko::vec2 velocity;
	float damping = 0;
	kiko::Color color;
	float lifetimer = 0;
	float lifetime = 0;
};

class Particle
{
public:
	Particle() = default;
	void Initialize(const ParticleData& data)
	{
		m_data = data;
		m_isActive = true;
	}
	void Update(float dt);
	void Draw(kiko::Renderer& renderer);
	friend class ParticleSystem;
	bool m_isActive = false;
private:
	ParticleData m_data;
};