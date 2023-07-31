#include "Core/Core.h"
#include "Core/Time.h"
#include "Renderer/Renderer.h"
#include "Renderer/ModelManager.h"
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "Player.h"
#include "Enemy.h"
#include "Framework/Scene.h"
#include "Framework/Emitter.h"
#include "Renderer/ParticleSystem.h"
#include "KHDarkFlight.h"

#include <iostream>
#include <vector>
#include <memory>
#include "Renderer/Font.h"
#include "Renderer/Text.h"

using namespace std;

class Star {
public:
	Star(const kiko::vec2& pos, const kiko::vec2& vel):
		m_pos{ pos },
		m_vel{ vel }
	{}

	void Update(int width, int height) {
		m_pos += m_vel * kiko::g_time.GetDeltaTime();

		if (m_pos.x >= width) m_pos.x = 0;
		if (m_pos.y >= height) m_pos.y = 0;
	}

	void Draw(kiko::Renderer& renderer) {
		renderer.DrawPoint(m_pos.x, m_pos.y);
	}
public:
	kiko::Vector2 m_pos;
	kiko::Vector2 m_vel;
};

int main(int argc, char* argv[])
{
	std::unique_ptr<int> up = std::make_unique<int>(10);

	int m1 = kiko::Max(4.0f, 3.0f);
	int m2 = kiko::Max(4, 3);
	int m3 = kiko::Max(4L, 3L);

	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("assets");

	kiko::g_audioSystem.Initialize();

	kiko::g_renderer.Initialize();
	kiko::g_renderer.CreateWindow("CSC196", 800, 600);

	kiko::g_inputSystem.Initialize();

	unique_ptr<KHDarkFlight> game = make_unique<KHDarkFlight>();
	game->Initialize();
	
	//std::vector<kiko::vec2> points{ { -10, 5 }, { 10, 5 }, { 0,-5 }, { -10, 5 } };

	kiko::vec2 v(5, 5);
	v.Normalize();

	vector<Star> stars;
	for (int i = 0; i < 1000; i++) {
		kiko::Vector2 pos(kiko::Vector2(kiko::random(kiko::g_renderer.GetWidth()), kiko::random(kiko::g_renderer.GetHeight())));
		kiko::Vector2 vel(kiko::randomf(100, 40), 0.0f);
		stars.push_back(Star(pos, vel));
	}

	kiko::Transform transform { { 400, 300}, 0, 3};

	float turnrate = kiko::DegreestoRadians(180);
	float speed = 100;

	bool quit = false;
	while (!quit) {

		kiko::g_time.Tick();
		kiko::g_inputSystem.Update();
		kiko::g_audioSystem.Update();
		kiko::g_particleSystem.Update(kiko::g_time.GetDeltaTime());
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_ESCAPE))
		{
			quit = true;
		}

		game->Update(kiko::g_time.GetDeltaTime());

		kiko::g_renderer.SetColor(0, 0, 0, 0);
		kiko::g_renderer.BeginFrame();

		kiko::Vector2 vel(1.0f, 0.3f);

		for (auto& star : stars) {
			star.Update(kiko::g_renderer.GetWidth(), kiko::g_renderer.GetHeight());

			kiko::g_renderer.SetColor(kiko::random(256), kiko::random(256), kiko::random(256), 255);
			star.Draw(kiko::g_renderer);
		}

		kiko::g_particleSystem.Draw(kiko::g_renderer);
		game->Draw(kiko::g_renderer);

		kiko::g_renderer.EndFrame();
	}

	stars.clear();
	kiko::g_memoryTracker.DisplayInfo();

	return 0;
}