#include "Texture.h"
#include "Renderer.h"

#include "Core/Core.h"

#include "SDL2-2.28.0/include/SDL_image.h"
namespace kiko
{
	Texture::~Texture()
	{
		if (m_texture) SDL_DestroyTexture(m_texture);
	}

	bool Texture::Create(std::string filename, ...)
	{
		va_list args;

		va_start(args, filename);

		Renderer& renderer = va_arg(args, Renderer);

		va_end(args);

		return Load(filename, renderer);
	}

	bool Texture::Load(const std::string& filename, Renderer& renderer)
	{
		SDL_Surface* surface = IMG_Load(filename.c_str());
		if (!surface)
		{
			WARNING_LOG("Surface is empty...")
			return false;
		}
		m_texture = SDL_CreateTextureFromSurface(renderer.m_renderer, surface);
		SDL_FreeSurface(surface);
		if (!m_texture)
		{
			WARNING_LOG("Texture is empty...")
			return false;
		}
		return true;
	}

	vec2 Texture::GetSize()
	{
		ASSERT_LOG(m_texture, "Can't get size as there is no texture...");
		SDL_Point point;
		SDL_QueryTexture(m_texture, nullptr, nullptr, &point.x, &point.y);
		return vec2{ point.x, point.y };
	}
}