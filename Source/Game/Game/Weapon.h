#pragma once
#include "Framework/Actor.h"

namespace kiko {
    class Weapon : public kiko::Actor
    {
    public:
        CLASS_DECLARATION(Weapon)

        bool Initialize() override;
        void Update(float dt) override;
        /*void OnCollision(Actor* other) override;*/

    private:
        float speed = 0;
        float m_turnRate = 0;
    };
}