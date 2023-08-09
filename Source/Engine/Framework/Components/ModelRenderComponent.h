#pragma once
#include "RenderComponent.h"
#include "Renderer/Texture.h"

namespace kiko {
	class ModelrnderComponent : public RenderComponent {
	public:
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	public:
		res_t<Texture> m_texture;
	};
}