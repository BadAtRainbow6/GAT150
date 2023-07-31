#pragma once
#include "Framework/Actor.h"

class Enemy : public kiko::Actor
{
public:
	enum class eType {
		Heartless,
		Nobody
	};
public:
	Enemy(const kiko::Transform& transform, std::shared_ptr<kiko::Model> model, eType type) :
		Actor{ transform, model }
	{
		m_type = type;
		m_turnrate = kiko::Pi;
		switch (m_type) {
		case eType::Nobody:
			m_health = 6;
			m_speed = 75.0f;
			break;
		case eType::Heartless:
			m_health = 1;
			m_speed = 100.0f;
			m_fireRate = 2.0f;
			m_fireTimer = m_fireRate;
		}
	}
	void Update(float dt) override;
	void OnCollision(Actor* other) override;

	bool GetFacingPlayer() const { return m_facingPlayer; }
	void SetFacingPlayer(bool facingPlayer) { m_facingPlayer = facingPlayer; }
private:
	int m_health = 0;
	eType m_type;

	float m_speed = 0;
	float m_turnrate = 0;

	float m_fireRate = 0;
	float m_fireTimer = 0;

	bool m_facingPlayer = false;
};
