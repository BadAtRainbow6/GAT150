#include "Sprite.h"
#include "Renderer/Renderer.h"
#include "Framework/Actor.h"

namespace kiko {
	void Sprite::Update(float dt)
	{
	}

	void Sprite::Draw(Renderer& renderer)
	{
		renderer.DrawTexture(m_texture.get(), m_owner->m_transform.position.x, m_owner->m_transform.position.y, RadiansToDegrees(m_owner->m_transform.rotation));
	}
}