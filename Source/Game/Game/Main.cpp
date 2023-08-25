#include "Core/Core.h"
#include "Core/Time.h"

#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "Player.h"
#include "Enemy.h"

#include "Framework/Framework.h"

#include "Renderer/Renderer.h"

#include "KHDarkFlight.h"

#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <map>
#include <functional>

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

void print(int count, ...)
{
	va_list args;

	va_start(args, count);
	for (int i = 0; i < count; ++i)
	{
		std::cout << va_arg(args, const char*) << std::endl;
	}
	va_end(args);
}

void print(int i) {
	cout << i << endl;
}

void zero(int& v) {
	v = 0;
}

void zero(int* v) {
	*v = 0;
}

void zero_ref(int& v) {
	cout << v << endl;
}

void print(const std::string& s) {
	cout << s << endl;
}

class A {
public:
	static A& Instance() {
		static A instance;
		return instance;
	}
	
	int getInt() { return 5; }

private:
	A() = default;
};

int getInt() {
	int i = 0;
	i++;

	return i;
}

int main(int argc, char* argv[])
{
	kiko::Factory::Instance().Register<kiko::Sprite>("Sprite");

	INFO_LOG("Start Game...");

	kiko::MemoryTracker::Initialize();
	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("assets");

	rapidjson::Document document;
	kiko::Json::Load("json.txt", document);
	int i1;
	kiko::Json::Read(document, "integer1", i1);
	std::cout << i1 << std::endl;
	int i2;
	kiko::Json::Read(document, "integer2", i2);
	std::cout << i2 << std::endl;

	std::string str;
	kiko::Json::Read(document, "string", str);
	std::cout << str << std::endl;
	bool b;
	kiko::Json::Read(document, "boolean", b);
	std::cout << b << std::endl;
	float f;
	kiko::Json::Read(document, "float", f);
	std::cout << f << std::endl;
	kiko::vec2 v2;
	kiko::Json::Read(document, "vector2", v2);
	std::cout << v2 << std::endl;

	kiko::g_audioSystem.Initialize();

	kiko::g_renderer.Initialize();
	kiko::g_renderer.CreateWindow("CSC196", 800, 600);

	kiko::g_inputSystem.Initialize();

	unique_ptr<KHDarkFlight> game = make_unique<KHDarkFlight>();
	game->Initialize();

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