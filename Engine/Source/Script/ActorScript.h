#pragma once

#include "Base.h"
#include "Scene/Actor.h"
#include "Scene/Scene.h"

namespace Core
{
    class CE_API ActorScript
    {
    public:
        Actor *Owner = nullptr;

        ActorScript() {};
        virtual ~ActorScript() {};

        Transform *GetTransform()
        {
            return Owner->GetTransform();
        };

        template <typename T>
        T *GetComponent() { return Owner->GetComponent<T>(); };

        template <typename T>
        T *GetComponents() { return Owner->GetComponents<T>(); };

        template <typename T>
        bool HasComponent() { return Owner->HasComponent<T>(); };

        template <typename T>
        void RemoveComponent() { Owner->RemoveComponent<T>(); };

        template <typename T>
        void RemoveComponent(int index) { Owner->RemoveComponent<T>(index); };

        template <typename T>
        void RemoveComponents() { Owner->RemoveComponent<T>(); };

        Scene *GetOwnerScene() { return Owner->GetScene(); };

        Actor *GetActorInScene(const std::string &name) { return GetOwnerScene()->GetActor(name); };
        Actor *GetActorInScene(const UUID &uid) { return GetOwnerScene()->GetActor(uid); };
        Actor *GetActorInSceneByTag(const std::string &tag) { return GetOwnerScene()->GetActorByTag(tag); };

        void RemoveActorInScene(const std::string &name) { GetOwnerScene()->RemoveActor(name); };
        void RemoveActorInScene(const UUID &uid) { GetOwnerScene()->RemoveActor(uid); };

        virtual void OnStart() {};
        virtual void OnUpdate() {};
        virtual void OnStop() {};
    };
}