#pragma once
#include "Object.h"
#include "Components/Component.h"
#include "Renderer/Renderer.h"
#include <memory>

namespace kiko
{
    class Actor : public Object
    {
    public:
        CLASS_DECLARATION(Actor)

        Actor() = default;
        Actor(const kiko::Transform& transform) :
            transform{ transform }
        {}
        Actor(const Actor& other);

        virtual bool Initialize() override;
        virtual void OnDestroy() override;

        virtual void Update(float dt);
        virtual void Draw(Renderer& renderer);

        void AddComponent(std::unique_ptr<Component> component);
        template<typename T>
        T* GetComponent();

        float GetRadius() { return 30; }
        virtual void OnCollision(Actor* other) {}

        class Scene* m_scene = nullptr;
        friend class Scene;
        friend class Game;

        class Game* m_game = nullptr;

    public:
        kiko::Transform transform;
        std::string tag;
        float lifespan = -1.0f; // -1.0 = no lifespan
        bool persistant = false;
        bool prototype = false;
        bool destroyed = false;

    protected:
        std::vector<std::unique_ptr<Component>> components;
    };

    template<typename T>
    inline T* Actor::GetComponent()
    {
        for (auto& component : components) {
            T* result = dynamic_cast<T*> (component.get());
            if (result) return result;
        }
        return nullptr;
    }
}

