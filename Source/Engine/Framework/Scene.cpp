#include "Scene.h"
#include "Core/Logger.h"
#include "Framework/Components/CollisionComponent.h"

namespace kiko
{
    bool Scene::Initialize() {
        for (auto& actor : actors) actor->Initialize();
        return true;
    }

    void kiko::Scene::Update(float dt)
    {
        auto iter = actors.begin();
        while (iter != actors.end()) {
            if ((*iter)->active) (*iter)->Update(dt);
            ((*iter)->destroyed) ? iter = actors.erase(iter) : iter++;
        }

        /*for (auto iter1 = m_actors.begin(); iter1 != m_actors.end(); iter1++) {
            for (auto iter2 = m_actors.begin(); iter2 != m_actors.end(); iter2++) {
                CollisionComponent* collision1 = (*iter1)->GetComponent<CollisionComponent>();
                CollisionComponent* collision2 = (*iter2)->GetComponent<CollisionComponent>();

                if (!collision1 || !collision2) continue;

                if (collision1->CheckCollision(collision2)) {
                    (*iter1)->OnCollision(iter2->get());
                    (*iter2)->OnCollision(iter1->get());
                }
            }
        }*/
    }

    void kiko::Scene::Draw(Renderer& renderer)
    {
        for (auto& actor : actors)
        {
            if(actor->active) actor->Draw(renderer);
        }
    }

    void kiko::Scene::Add(std::unique_ptr<Actor> actor)
    {
        actor->m_scene = this;
        actors.push_back(std::move(actor));
    }

    void kiko::Scene::RemoveAll(bool force)
    {
        auto iter = actors.begin();
        while (iter != actors.end()) {
            (force || !(*iter)->persistant) ? iter = actors.erase(iter) : iter++;
        }
    }
    bool Scene::Load(const std::string& filename)
    {
        rapidjson::Document document;

        if (!Json::Load(filename, document)) {
            ERROR_LOG("Could not load scene file: " << filename);
            return false;
        }
        Read(document);
        return true;
    }
    void Scene::Read(const json_t& value)
    {
        if (HAS_DATA(value, actors) && GET_DATA(value, actors).IsArray()) {
            for (auto& actorValue : GET_DATA(value, actors).GetArray()) {
                std::string type;
                READ_DATA(actorValue, type);

                auto actor = CREATE_CLASS_BASE(Actor, type);
                actor->Read(actorValue);

                if (actor->prototype) {
                    std::string name = actor->name;
                    Factory::Instance().RegisterPrototype(name, std::move(actor));
                }
                else {
                    Add(std::move(actor));
                }

            }
        }
    }
}