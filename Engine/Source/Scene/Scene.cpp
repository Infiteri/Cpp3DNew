#include "Scene.h"
#include "Core/Logger.h"

#include <algorithm>

namespace Core
{
    Scene::Scene()
    {
        environemnt.Setup();
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

        if (actor->GetState() != Actor::Started)
            actor->Start();

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

    Actor *Scene::GetActorInHierarchy(const UUID &uuid)
    {
        Actor *a = GetActorByUUID(uuid);

        if (!a)
        {
            for (auto actor : actors)
            {
                if (a)
                    break;
                a = actor->FindChildInHierarchyByUUID(uuid);
            }
        }

        return a;
    }

    void Scene::RemoveActorByUUID(const UUID &uid)
    {
        Actor *a;

        int index = -1;
        for (auto actor : actors)
        {
            index++;
            if (actor->id == uid)
            {
                actors.erase(actors.begin() + index);
                break;
            }
        }
    }

    void Scene::MoveChildInHierarchy(const UUID &uid, int newIndex)
    {

        Actor *actorToMove = GetActorByUUID(uid);

        if (!actorToMove)
        {
            return;
        }

        if (newIndex < 0 || newIndex >= actors.size())
        {
            return;
        }

        // Find the current index of the actor
        auto actorIterator = std::find(actors.begin(), actors.end(), actorToMove);

        if (actorIterator != actors.end())
        {
            size_t currentIndex = std::distance(actors.begin(), actorIterator);

            // Remove the actor from the current position
            actors.erase(actorIterator);

            // Insert the actor at the new index
            actors.insert(actors.begin() + newIndex, actorToMove);
        }
    }
}