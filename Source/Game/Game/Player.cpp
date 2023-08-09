#include "Player.h"
#include "Weapon.h"
#include "KHDarkFlight.h"
#include "Framework/Scene.h"
#include "Framework/Emitter.h"
#include "Renderer/Renderer.h"
#include "Framework/Components/Sprite.h"
#include "Framework/Resource/ResourceManager.h"
#include "Framework/Components/PhysicsComponent.h"
#include "Renderer/Texture.h"
#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"

void Player::Update(float dt)
{
	Actor::Update(dt);

	float rotate = 0;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
	m_transform.rotation += rotate * m_turnrate * kiko::g_time.GetDeltaTime();

	float thrust = 0;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) thrust = 1;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_S)) thrust = -1;

	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(m_transform.rotation);

	auto physicsComponent = GetComponent<kiko::PhysicsComponent>();
	physicsComponent->ApplyForce(forward * m_speed * thrust);

	m_transform.position += forward * m_speed * thrust * kiko::g_time.GetDeltaTime();

	m_transform.position.x = kiko::Wrap((float)m_transform.position.x, (float)kiko::g_renderer.GetWidth());
	m_transform.position.y = kiko::Wrap((float)m_transform.position.y, (float)kiko::g_renderer.GetHeight());

	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_RETURN) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_RETURN)) {

		std::unique_ptr<kiko::Sprite> component = std::make_unique<kiko::Sprite>();
		component->m_texture = kiko::g_resources.Get<kiko::Texture>("ship.png", kiko::g_renderer);

		kiko::Transform transform1 {m_transform.position, m_transform.rotation + kiko::DegreestoRadians(-30.0f), 1};
		std::unique_ptr<Weapon> weapon1 = std::make_unique<Weapon>(400, transform1, 0.3f);
		weapon1->m_tag = "Player";
		weapon1->AddComponent(std::move(component));

		kiko::Transform transform2 {m_transform.position, m_transform.rotation + kiko::DegreestoRadians(-20.0f), 1};
		std::unique_ptr<Weapon> weapon2 = std::make_unique<Weapon>(400, transform2, 0.3f);
		weapon2->m_tag = "Player";
		weapon2->AddComponent(std::move(component));

		kiko::Transform transform3 {m_transform.position, m_transform.rotation + kiko::DegreestoRadians(-10.0f), 1};
		std::unique_ptr<Weapon> weapon3 = std::make_unique<Weapon>(400, transform3, 0.3f);
		weapon3->m_tag = "Player";
		weapon3->AddComponent(std::move(component));

		kiko::Transform transform4 {m_transform.position, m_transform.rotation, 1};
		std::unique_ptr<Weapon> weapon4 = std::make_unique<Weapon>(400, transform4, 0.3f);
		weapon4->m_tag = "Player";
		weapon4->AddComponent(std::move(component));

		kiko::Transform transform5 {m_transform.position, m_transform.rotation + kiko::DegreestoRadians(10.0f), 1};
		std::unique_ptr<Weapon> weapon5 = std::make_unique<Weapon>(400, transform5, 0.3f);
		weapon5->m_tag = "Player";
		weapon5->AddComponent(std::move(component));

		kiko::Transform transform6 {m_transform.position, m_transform.rotation + kiko::DegreestoRadians(20.0f), 1};
		std::unique_ptr<Weapon> weapon6 = std::make_unique<Weapon>(400, transform6, 0.3f);
		weapon6->m_tag = "Player";
		weapon6->AddComponent(std::move(component));

		kiko::Transform transform7 {m_transform.position, m_transform.rotation + kiko::DegreestoRadians(30.0f), 1};
		std::unique_ptr<Weapon> weapon7 = std::make_unique<Weapon>(400, transform7, 0.3f);
		weapon7->m_tag = "Player";
		weapon7->AddComponent(std::move(component));

		kiko::g_audioSystem.PlayOneShot("hit");
	}

	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE)) {
		kiko::Transform transform {m_transform.position, m_transform.rotation, 1};
		std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400, transform, 1.0f);
		weapon->m_tag = "Player";

		std::unique_ptr<kiko::Sprite> component = std::make_unique<kiko::Sprite>();
		component->m_texture = kiko::g_resources.Get<kiko::Texture>("ship.png", kiko::g_renderer);
		weapon->AddComponent(std::move(component));

		weapon->AddComponent(std::move(component));
		kiko::g_audioSystem.PlayOneShot("hit");
	}

	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_T)) kiko::g_time.SetTimeScale(0.5f);
	else kiko::g_time.SetTimeScale(1.0f);
}

void Player::OnCollision(Actor* other) {
	if (other->m_tag == "Enemy" && !m_destroyed) {
		m_destroyed = true;
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
		kiko::Transform transform{this->m_transform.position, 0, 1 };
		auto emitter = std::make_unique<kiko::Emitter>(transform, data);
		emitter->m_lifespan = 1.0f;
		m_scene->Add(std::move(emitter));
	}
}
