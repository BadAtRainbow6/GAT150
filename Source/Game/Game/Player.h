#pragma once
#include "Framework/Framework.h"
#include "Framework/Actor.h"

namespace kiko {
	class Player : public kiko::Actor
	{
	public:
		CLASS_DECLARATION(Player);
		Player(float speed, float turnrate, const kiko::Transform& transform) :
			Actor{ transform },
			m_speed{ speed },
			m_turnrate{ turnrate }
		{}
		bool Initialize() override;
		void Update(float dt) override;
		void OnCollision(Actor* other) override;

	private:
		float m_speed = 0;
		float m_turnrate = 0;
		float m_health = 100;

		kiko::PhysicsComponent* m_physicsComponent = nullptr;
	};
}
