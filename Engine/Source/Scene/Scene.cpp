#include "Scene.h"
#include "Core/Logger.h"

#include "Renderer/Texture/TextureSystem.h"
#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Shader/ShaderSystem.h"
#include "Renderer/Light/LightID.h"
#include "Components/Components.h"

#include "Script/ScriptEngine.h"

#include <algorithm>

namespace Core
{
    void Scene::LoadRuntimeComponent(Actor *a)
    {
        for (auto script : a->GetComponents<ScriptComponent>())
            ScriptEngine::RegisterScript(a->GetName(), script->ClassName, a);
    }

    Scene::Scene()
    {
        state = Initialized;
        environemnt.Setup();
    }

    Scene::~Scene()
    {
        for (auto actor : actors)
            delete actor;

        actors.clear();
    }

    Scene *Scene::From(Scene *scene)
    {
        Scene *out = new Scene();
        out->environemnt.From(&scene->environemnt);
        out->name = scene->name;

        for (auto actor : scene->GetActors())
        {
            Actor *a = Actor::From(actor);
            out->AddActor(a);
        }

        return out;
    }

    void Scene::Start()
    {
        state = Started;

        for (auto actor : actors)
        {
            LoadRuntimeComponent(actor);
        }

        for (auto actor : actors)
        {
            actor->Start();
        }

        // Post load section
        // All actors are ready, post component loading and actor setup section
        // Things like scripting and physics setup goes here
        ActivateSceneCamera();
        ScriptEngine::StartRuntime();
    }

    void Scene::Render()
    {
        state = Running;
        TextureSystem::StartNewSceneGeneration();

        environemnt.DirectionalLight.Render();

        for (auto actor : actors)
        {
            actor->Render();
        }

        auto objectShader = ShaderSystem::GetFromEngineResource("Object");
        if (objectShader)
        {
            objectShader->Int(LightID::GetPointLight(), "uPointLightCount");
        }

        LightID::Begin();
    }

    void Scene::Update()
    {
        state = Running;
        for (auto actor : actors)
        {
            actor->Update();
        }

        ScriptEngine::UpdateRuntime();
    }

    void Scene::Stop()
    {
        state = Stopped;

        for (auto actor : actors)
        {
            actor->Stop();
        }

        ScriptEngine::StopRuntime();
    }

    void Scene::AddActor(Actor *actor)
    {
        if (!actor)
        {
            CE_ERROR("Actor invalid.");
            return;
        }

        if (state == Running || state == Started)
        {
            actor->Start();
            LoadRuntimeComponent(actor);
        }

        actor->scene = this;
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

    void Scene::SetPrimaryCameraToNone()
    {
        for (auto a : actors)
        {
            for (auto c : a->GetComponents<CameraComponent>())
            {
                c->IsPrimary = false;
            }
        }
    }

    void Scene::ActivateSceneCamera()
    {
        CameraComponent *c = GetPrimaryCameraComponent();
        if (!c)
            c = GetFirstCameraComponent();

        if (!c)
            CameraSystem::Activate(nullptr);
        else
            CameraSystem::Activate(c->Camera);
    }

    CameraComponent *Scene::GetPrimaryCameraComponent()
    {
        CameraComponent *ret = nullptr;
        for (auto a : actors)
        {
            for (auto c : a->GetComponents<CameraComponent>())
            {
                if (c->IsPrimary)
                {
                    if (!ret)
                        ret = c;
                }
            }
        }

        return ret;
    }

    CameraComponent *Scene::GetFirstCameraComponent()
    {
        for (auto a : actors)
        {
            for (auto c : a->GetComponents<CameraComponent>())
            {
                return c;
            }
        }

        return nullptr;
    }
}