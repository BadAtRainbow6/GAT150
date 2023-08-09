#pragma once
#include "Framework/Actor.h"

class Weapon : public kiko::Actor
{
public:
    Weapon() = default;
    Weapon(float speed, const kiko::Transform& transform, float lifespan) :
        Actor{ transform },
        m_speed{ speed }
    {
        m_lifespan = lifespan;
    }

    void Update(float dt) override;
    void OnCollision(Actor* other) override;

private:
    float m_speed = 0;
    float m_turnRate = 0;
};