#include "Particle.h"

void Particle::Update(float dt)
{
	m_data.lifetimer += dt;
	if (m_data.lifetimer >= m_data.lifetime)
	{
		m_isActive = false;
		return;
	}
	m_data.prevPosition = m_data.position;
	m_data.position += m_data.velocity * dt;
	m_data.velocity *= std::pow(1.0f - m_data.damping, dt);
}

void Particle::Draw(kiko::Renderer& renderer)
{
	renderer.SetColor(kiko::Color::ToInt(m_data.color.r), kiko::Color::ToInt(m_data.color.g), kiko::Color::ToInt(m_data.color.b), kiko::Color::ToInt(m_data.color.a));
	renderer.DrawLine(m_data.position.x, m_data.position.y, m_data.prevPosition.x, m_data.prevPosition.y);
}