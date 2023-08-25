#include "Sprite.h"
#include "Renderer/Renderer.h"
#include "Framework/Actor.h"
#include "Framework/Resource/ResourceManager.h"

namespace kiko {

	CLASS_DEFINITION(Sprite);

	bool Sprite::Initialize()
	{
		if(!textureName.empty()) m_texture = GET_RESOURCE(Texture, textureName, g_renderer);
		return true;
	}

	void Sprite::Update(float dt)
	{
	}

	void Sprite::Draw(Renderer& renderer)
	{
		renderer.DrawTexture(m_texture.get(), m_owner->transform);
	}

	void Sprite::Read(const json_t& value) {
		READ_DATA(value, textureName);
	}
}