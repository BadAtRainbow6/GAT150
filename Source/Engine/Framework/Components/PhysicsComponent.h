#pragma once
#include "Core/Math/Vector2.h"
#include "Component.h"

namespace kiko {
	class PhysicsComponent : public Component {
	public:
		virtual void ApplyForce(vec2 force) = 0;

	public:
		vec2 m_velocity;
		vec2 m_acceleration;
		float m_mass = 1.0f;
		float m_damping = 0;

	};
}