#include "KHDarkFlight.h"
#include "Player.h"
#include "Enemy.h"
#include "Renderer/Renderer.h"

#include "Framework/Framework.h"
#include "Core/Core.h"
#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"


bool KHDarkFlight::Initialize()
{
	m_font = GET_RESOURCE(kiko::Font, "aYummyApology.ttf", 24);
	m_scoreText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "aYummyApology.ttf", 24));
	m_scoreText->Create(kiko::g_renderer, "SCORE 0000", kiko::Color{ 1, 0, 1, 1 });

	m_livesText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "aYummyApology.ttf", 24));
	m_livesText->Create(kiko::g_renderer, "Lives: 0", kiko::Color{ 1, 1, 1, 1 });

	m_titleText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "aYummyApology.ttf", 24));
	m_titleText->Create(kiko::g_renderer, "Kingdom Hearts: Dark Flight", kiko::Color{ 1, 1, 1, 1 });

	m_gameOverText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "aYummyApology.ttf", 24));
	m_gameOverText->Create(kiko::g_renderer, "Game Over", kiko::Color{ 1, 1, 1, 1 });

	m_timerText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "aYummyApology.ttf", 24));
	m_timerText->Create(kiko::g_renderer, std::to_string(m_gameTimer), kiko::Color{ 1, 1, 1, 1 });

	kiko::g_audioSystem.AddAudio("hit", "Hit.wav");
	kiko::g_audioSystem.AddAudio("gummiShip", "gummiShip.mp3");

	m_scene = std::make_unique<kiko::Scene>();
	m_scene->Load("scene.json");
	m_scene->Initialize();

	return false;
}

void KHDarkFlight::Shutdown()
{
}

void KHDarkFlight::Update(float dt)
{
	switch (m_state) {
	case KHDarkFlight::eState::Title:
		m_scene->GetActorByName<kiko::Actor>("Title")->active = true;
		kiko::g_audioSystem.PlayOneShot("gummiShip", true);
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE))
		{
			m_state = eState::StartGame;
			m_scene->GetActorByName<kiko::Actor>("Title")->active = false;
		}
		break;
	case KHDarkFlight::eState::StartGame:
		m_score = 0;
		m_lives = 3;
		m_state = eState::StartLevel;
		break;
	case KHDarkFlight::eState::StartLevel:
		m_scene->RemoveAll();
	{
		//create player
		std::unique_ptr<kiko::Player> player = std::make_unique <kiko::Player>(20.0f, kiko::Pi, kiko::Transform{ { 400, 300}, 0, 1.0f });
		player->tag = "Player";
		player->m_game = this;
		//create components
		auto renderComponent = CREATE_CLASS(Sprite);
		renderComponent->m_texture = GET_RESOURCE(kiko::Texture, "ship.png", kiko::g_renderer);
		player->AddComponent(std::move(renderComponent));

		auto physicsComponent = CREATE_CLASS(EnginePhysicsComponent);
		physicsComponent->m_damping = 0.9f;
		player->AddComponent(std::move(physicsComponent));

		auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
		collisionComponent->m_radius = 30.0f;
		player->AddComponent(std::move(collisionComponent));

		player->Initialize();
		m_scene->Add(std::move(player));
	}
		m_state = eState::Game;
		break;
	case KHDarkFlight::eState::Game:
		m_spawnTimer += dt;
		if (m_spawnTimer >= m_spawnTime) {
			m_spawnTimer = 0;
			std::unique_ptr<Enemy> enemy;
			auto renderComponent = std::make_unique<kiko::Sprite>();
			auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			switch (kiko::random(0, 1)) {
			case 0:
				enemy = std::make_unique<Enemy>(kiko::Transform{ { kiko::random(600), kiko::random(600)}, kiko::randomf(kiko::DoublePi), 3}, Enemy::eType::Heartless);
				renderComponent->m_texture = GET_RESOURCE(kiko::Texture, "heartless.png", kiko::g_renderer);
				enemy->AddComponent(std::move(renderComponent));

				collisionComponent->m_radius = 30.0f;
				enemy->AddComponent(std::move(collisionComponent));
				break;
			case 1:
				enemy = std::make_unique<Enemy>(kiko::Transform{ { kiko::random(600), kiko::random(600)}, kiko::randomf(kiko::DoublePi), 3}, Enemy::eType::Nobody);
				renderComponent->m_texture = GET_RESOURCE(kiko::Texture, "nobody.png", kiko::g_renderer);
				enemy->AddComponent(std::move(renderComponent));

				collisionComponent->m_radius = 30.0f;
				enemy->AddComponent(std::move(collisionComponent));
				break;
			}
			enemy->tag = "Enemy";
			enemy->m_game = this;
			m_scene->Add(std::move(enemy));
		}
		break;
	case KHDarkFlight::eState::PlayerDeadStart:
		m_stateTimer = 3;
		if (m_lives <= 0) m_state = eState::GameOverStart;
		else m_state = eState::PlayerDead;
		break;
	case KHDarkFlight::eState::PlayerDead:
		m_stateTimer -= dt;
		if (m_stateTimer <= 0) {
			if (m_lives == 0) {
				m_state = eState::GameOverStart;
			}
			else m_state = eState::StartLevel;
		}
		break;
	case KHDarkFlight::eState::GameOverStart:
		m_stateTimer -= dt;
		if (m_stateTimer <= 0) {
			m_stateTimer = 3;
			m_scene->RemoveAll();
			m_state = eState::GameOver;
		}
		break;
	case KHDarkFlight::eState::GameOver:
		m_stateTimer -= dt;
		if (m_stateTimer <= 0) {
			m_state = eState::Title;
		}
		break;
	}

	m_livesText->Create(kiko::g_renderer, "Lives: " + std::to_string(m_lives), { 1, 1, 1, 1 });
	m_scoreText->Create(kiko::g_renderer, "Score: " + std::to_string(m_score), {1, 1, 1, 1});
	m_scene->Update(dt);
}

void KHDarkFlight::Draw(kiko::Renderer& renderer)
{
	m_scene->Draw(renderer);
	if (m_state == eState::GameOver) {
		m_gameOverText->Draw(renderer, 300, 300);
	}

	m_livesText->Draw(renderer, 540, 40);
	m_scoreText->Draw(renderer, 40, 40);
}
