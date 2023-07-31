#pragma once
#include "Framework/Actor.h"

class Player : public kiko::Actor
{
public:
	Player(float speed, float turnrate, const kiko::Transform& transform, std::shared_ptr<kiko::Model> model) :
		Actor{ transform, model},
		m_speed{ speed },
		m_turnrate { turnrate }
	{}
	void Update(float dt) override;
	void OnCollision(Actor* other) override;

private:
	float m_speed = 0;
	float m_turnrate = 0;
	float m_health = 100;
};

