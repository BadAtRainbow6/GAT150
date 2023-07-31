#include "Enemy.h"
#include "Player.h"
#include "KHDarkFlight.h"
#include "Weapon.h"
#include "Framework/Emitter.h"
#include "Framework/Scene.h"
#include "Renderer/Renderer.h"
#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"

void Enemy::Update(float dt) {
	Actor::Update(dt);

	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(m_transform.rotation);
	Player* player = m_scene->GetActor<Player>();
	if (player) {
		kiko::Vector2 direction = player->m_transform.position - m_transform.position;

		float turnAngle = kiko::vec2::SignedAngle(forward, direction.Normalized());
		m_transform.rotation += turnAngle * dt;

		if (std::fabs(turnAngle) < kiko::DegreestoRadians(30.0f)) {
			SetFacingPlayer(true);
		}
	}
	m_transform.position += forward * m_speed * kiko::g_time.GetDeltaTime();

	m_transform.position.x = kiko::Wrap((float)m_transform.position.x, (float)kiko::g_renderer.GetWidth());
	m_transform.position.y = kiko::Wrap((float)m_transform.position.y, (float)kiko::g_renderer.GetHeight());

	m_fireTimer -= kiko::g_time.GetDeltaTime();

	if (m_fireTimer <= 0 && GetFacingPlayer() && m_type != eType::Nobody) {
		kiko::Transform transform {m_transform.position, m_transform.rotation, 1};
		std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(150, transform, m_model, 3.0f);
		weapon->m_tag = "Enemy";
		m_scene->Add(std::move(weapon));
		kiko::g_audioSystem.PlayOneShot("hit");

		m_fireTimer = m_fireRate;
	}
	else {
		m_fireTimer -= kiko::g_time.GetDeltaTime();
	}
}

void Enemy::OnCollision(Actor* other) {
	if (other->m_tag == "Player" && !m_destroyed) {
		m_health--;
		if (m_health <= 0) {
			m_game->AddPoints(100);
			m_destroyed = true;

			kiko::EmitterData data;
			data.burst = true;
			data.burstCount = 100;
			data.spawnRate = 200;
			data.angle = 0;
			data.angleRange = kiko::Pi;
			data.lifetimeMin = 0.5f;
			data.lifetimeMin = 1.5f;
			data.speedMin = 50;
			data.speedMax = 250;
			data.damping = 0.5f;
			if (m_type == eType::Heartless) {
				data.color = kiko::Color{ 1, 0, 0, 1 };
			}
			else if (m_type == eType::Nobody) {
				data.color = kiko::Color{ 1, 1, 1, 1 };
			}
			kiko::Transform transform{this->m_transform.position, 0, 1 };
			auto emitter = std::make_unique<kiko::Emitter>(transform, data);
			emitter->m_lifespan = 1.0f;
			m_scene->Add(std::move(emitter));
		}
	}
}
