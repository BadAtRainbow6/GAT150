#include "Enemy.h"
#include "Player.h"
#include "KHDarkFlight.h"
#include "Weapon.h"
#include "Framework/Framework.h"
#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"

bool Enemy::Initialize() {
	Actor::Initialize();
	auto collisionComponent = GetComponent<kiko::CollisionComponent>();
	if (collisionComponent) {
		auto renderComponent = GetComponent<kiko::RenderComponent>();
		if (renderComponent) {
			float scale = transform.scale;
			collisionComponent->m_radius = renderComponent->GetRadius() * scale;
		}
	}
	return true;
}

void Enemy::Update(float dt) {
	Actor::Update(dt);

	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
	Player* player = m_scene->GetActor<Player>();
	if (player) {
		kiko::Vector2 direction = player->transform.position - transform.position;

		float turnAngle = kiko::vec2::SignedAngle(forward, direction.Normalized());
		transform.rotation += turnAngle * dt;

		if (std::fabs(turnAngle) < kiko::DegreestoRadians(30.0f)) {
			SetFacingPlayer(true);
		}
	}
	transform.position += forward * m_speed * kiko::g_time.GetDeltaTime();

	transform.position.x = kiko::Wrap((float)transform.position.x, (float)kiko::g_renderer.GetWidth());
	transform.position.y = kiko::Wrap((float)transform.position.y, (float)kiko::g_renderer.GetHeight());

	m_fireTimer -= kiko::g_time.GetDeltaTime();

	/*if (m_fireTimer <= 0 && GetFacingPlayer() && m_type != eType::Nobody) {
		kiko::Transform transform {transform.position, transform.rotation, 1};
		std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(150, transform, 3.0f);
		weapon->tag = "Enemy";
		m_scene->Add(std::move(weapon));
		kiko::g_audioSystem.PlayOneShot("hit");

		m_fireTimer = m_fireRate;
	}
	else {
		m_fireTimer -= kiko::g_time.GetDeltaTime();
	}*/
}

void Enemy::OnCollision(Actor* other) {
	if (other->tag == "Player" && !destroyed) {
		m_health--;
		if (m_health <= 0) {
			m_game->AddPoints(100);
			destroyed = true;

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
			kiko::Transform transform{this->transform.position, 0, 1 };
			auto emitter = std::make_unique<kiko::Emitter>(transform, data);
			emitter->lifespan = 1.0f;
			m_scene->Add(std::move(emitter));
		}
	}
}
