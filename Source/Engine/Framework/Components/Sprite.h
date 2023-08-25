#pragma once
#include "RenderComponent.h"
#include "Renderer/Texture.h"

namespace kiko {
	class Sprite : public RenderComponent {
	public:
		CLASS_DECLARATION(Sprite);

		bool Initialize() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

		virtual float GetRadius() { return m_texture->GetSize().Length() * 0.5f; }

	public:
		std::string textureName;
		res_t<Texture> m_texture;
	};
}