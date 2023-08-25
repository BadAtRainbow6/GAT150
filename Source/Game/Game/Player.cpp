#include "Player.h"
#include "Weapon.h"
#include "KHDarkFlight.h"
#include "Renderer/Renderer.h"
#include "Framework/Framework.h"
#include "Core/Core.h"
#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"

namespace kiko {
	
	CLASS_DEFINITION(Player)

	bool Player::Initialize() {
		Actor::Initialize();
		m_physicsComponent = GetComponent<kiko::PhysicsComponent>();
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

	void Player::Update(float dt)
	{
		Actor::Update(dt);

		float rotate = 0;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
		//transform.rotation += rotate * m_turnrate * kiko::g_time.GetDeltaTime();
		//m_physicsComponent->ApplyTorque(rotate * turnRate);

		float thrust = 0;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) thrust = 1;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_S)) thrust = -1;

		kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);

		m_physicsComponent->ApplyForce(forward * m_speed * thrust);

		transform.position += forward * m_speed * thrust * kiko::g_time.GetDeltaTime();

		transform.position.x = kiko::Wrap((float)transform.position.x, (float)kiko::g_renderer.GetWidth());
		transform.position.y = kiko::Wrap((float)transform.position.y, (float)kiko::g_renderer.GetHeight());

		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_RETURN) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_RETURN)) {
			auto weapon1 = INSTANTIATE(Weapon, "Bullet");
			weapon1->transform = { transform.position, transform.rotation + kiko::DegreestoRadians(-30.0f), 1 };
			weapon1->Initialize();
			m_scene->Add(std::move(weapon1));

			/*
			auto component = std::make_unique<kiko::Sprite>();
			component->m_texture = GET_RESOURCE(kiko::Texture, "ship.txt", kiko::g_renderer);
			auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 30.0f;

			kiko::Transform transform1 {transform.position, transform.rotation + kiko::DegreestoRadians(-30.0f), 1};
			std::unique_ptr<Weapon> weapon1 = std::make_unique<Weapon>(400, transform1, 0.3f);
			weapon1->tag = "Player";
			weapon1->AddComponent(std::move(component));
			weapon1->AddComponent(std::move(collisionComponent));

			kiko::Transform transform2 {transform.position, transform.rotation + kiko::DegreestoRadians(-20.0f), 1};
			std::unique_ptr<Weapon> weapon2 = std::make_unique<Weapon>(400, transform2, 0.3f);
			weapon2->tag = "Player";
			weapon2->AddComponent(std::move(component));
			weapon2->AddComponent(std::move(collisionComponent));

			kiko::Transform transform3 {transform.position, transform.rotation + kiko::DegreestoRadians(-10.0f), 1};
			std::unique_ptr<Weapon> weapon3 = std::make_unique<Weapon>(400, transform3, 0.3f);
			weapon3->tag = "Player";
			weapon3->AddComponent(std::move(component));
			weapon3->AddComponent(std::move(collisionComponent));

			kiko::Transform transform4 {transform.position, transform.rotation, 1};
			std::unique_ptr<Weapon> weapon4 = std::make_unique<Weapon>(400, transform4, 0.3f);
			weapon4->tag = "Player";
			weapon4->AddComponent(std::move(component));
			weapon4->AddComponent(std::move(collisionComponent));

			kiko::Transform transform5 {transform.position, transform.rotation + kiko::DegreestoRadians(10.0f), 1};
			std::unique_ptr<Weapon> weapon5 = std::make_unique<Weapon>(400, transform5, 0.3f);
			weapon5->tag = "Player";
			weapon5->AddComponent(std::move(component));
			weapon5->AddComponent(std::move(collisionComponent));

			kiko::Transform transform6 {transform.position, transform.rotation + kiko::DegreestoRadians(20.0f), 1};
			std::unique_ptr<Weapon> weapon6 = std::make_unique<Weapon>(400, transform6, 0.3f);
			weapon6->tag = "Player";
			weapon6->AddComponent(std::move(component));
			weapon6->AddComponent(std::move(collisionComponent));

			kiko::Transform transform7 {transform.position, transform.rotation + kiko::DegreestoRadians(30.0f), 1};
			std::unique_ptr<Weapon> weapon7 = std::make_unique<Weapon>(400, transform7, 0.3f);
			weapon7->tag = "Player";
			weapon7->AddComponent(std::move(component));
			weapon7->AddComponent(std::move(collisionComponent));

			weapon1->Initialize();
			weapon2->Initialize();
			weapon3->Initialize();
			weapon4->Initialize();
			weapon5->Initialize();
			weapon6->Initialize();
			weapon7->Initialize();
			kiko::g_audioSystem.PlayOneShot("hit");
			*/
		}

		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE)) {
			/*
			kiko::Transform transform {transform.position, transform.rotation, 1};
			std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400, transform, 1.0f);
			weapon->tag = "Player";

			auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 30.0f;
			weapon->AddComponent(std::move(collisionComponent));

			std::unique_ptr<kiko::Sprite> component = std::make_unique<kiko::Sprite>();
			component->m_texture = GET_RESOURCE(kiko::Texture, "ship.png", kiko::g_renderer);
			weapon->AddComponent(std::move(component));

			weapon->Initialize();
			kiko::g_audioSystem.PlayOneShot("hit");
			*/
		}

		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_T)) kiko::g_time.SetTimeScale(0.5f);
		else kiko::g_time.SetTimeScale(1.0f);
	}

	void Player::OnCollision(Actor* other) {
		if (other->tag == "Enemy" && !destroyed) {
			destroyed = true;
			m_game->SetLives(m_game->GetLives() - 1);
			dynamic_cast<KHDarkFlight*>(m_game)->SetState(KHDarkFlight::eState::PlayerDeadStart);

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
			data.color = kiko::Color{ 1, 1, 0, 1 };
			kiko::Transform transform{this->transform.position, 0, 1 };
			auto emitter = std::make_unique<kiko::Emitter>(transform, data);
			emitter->lifespan = 1.0f;
			m_scene->Add(std::move(emitter));
		}
	}
}