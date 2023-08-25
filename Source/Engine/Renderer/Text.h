#pragma once
#include "Font.h"
#include "Core/Math/Color.h"
#include <string>
#include <memory>

struct SDL_Texture;

namespace kiko
{
	class Text
	{
	public:
		Text();
		Text(std::shared_ptr<Font> font) : m_font{ font } { }
		~Text();
		void Create(class Renderer& renderer, const std::string& text, const Color& color);
		void Draw(class Renderer& renderer, int x, int y);
		void Draw(Renderer& renderer, const class Transform& transform);
	private:
		std::shared_ptr<Font> m_font;
		SDL_Texture* m_texture = nullptr;
	};
}