#include "Scene.h"
#include "Core/Logger.h"

namespace Core
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    void Scene::Start()
    {
        for (auto actor : actors)
        {
            actor->Start();
        }
    }

    void Scene::Render()
    {
        for (auto actor : actors)
        {
            actor->Render();
        }
    }

    void Scene::Update()
    {
        for (auto actor : actors)
        {
            actor->Update();
        }
    }

    void Scene::Stop()
    {
        for (auto actor : actors)
        {
            actor->Stop();
        }
    }

    void Scene::AddActor(Actor *actor)
    {
        if (!actor)
        {
            CE_ERROR("Actor invalid.");
            return;
        }

        actors.push_back(actor);
    }

    Actor *Scene::SpawnActor()
    {
        Actor *a = new Actor();
        AddActor(a);
        return a;
    }

    Actor *Scene::GetActorByName(const std::string &name)
    {
        for (auto a : actors)
            if (a->GetName() == name)
                return a;

        return nullptr;
    }

    Actor *Scene::GetActorByUUID(const UUID &uuid)
    {
        for (auto a : actors)
            if (a->GetUUID() == uuid)
                return a;

        return nullptr;
    }
}