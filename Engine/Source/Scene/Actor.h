#pragma once

#include "Base.h"
#include "Core/UUID.h"
#include "Math/Transform.h"
#include "Math/Matrix4.h"
#include "Components/Components.h"
#include <string>
#include <vector>

namespace Core
{
    typedef std::vector<Actor *> ActorList;

    class CE_API Actor
    {
    public:
        enum State
        {
            Initialized,
            Started,
            Running,
            Stopped
        };

    private:
        State state;
        UUID id;
        std::string name;

        friend class Scene;

        std::vector<Component *> components;

        // transform and stuff
        Transform transform;

        Matrix4 localMatrix;
        Matrix4 worldMatrix; // when parenting and stuff comes along

        void _CalculateMatrices();

        // children
        ActorList children;
        Actor *parent = nullptr;

        friend class SceneSerializer;
        friend class ComponentSerializer;

    public:
        Actor();
        ~Actor();

        static Actor *From(Actor *a);

        void SetName(const std::string &name) { this->name = name; };
        inline std::string GetName() { return name; };

        inline State GetState() { return state; };

        UUID GetUUID() { return UUID(id.Get()); };

        void Start();
        void Render();
        void Update();
        void Stop();

        inline Transform *GetTransform() { return &transform; };
        inline Matrix4 GetTransformMatrix() { return worldMatrix; };

        // --------- PARENTING ------------
        inline ActorList GetChildren() { return children; };
        inline Actor *GetParent() { return parent; };
        inline bool IsOwned() { return parent != nullptr; };
        inline bool SameUUID(const UUID &uuid) { return id == uuid; };
        inline bool SameUUID(Actor *a) { return id == a->id; };

        void AddChild(Actor *actor);
        Actor *SpawnChild();

        /// @brief Searches for an actor in the children list of this actor. Goes only one level deep.
        /// @param uuid The uuid.
        /// @return Actor * or nullptr.
        Actor *FindChildByUUID(const UUID &uuid);

        /// @brief Searches for an actor in the children list of this actor. Goes fully deep (recursive).
        /// @param uuid The uuid.
        /// @return Actor * or nullptr.
        Actor *FindChildInHierarchyByUUID(const UUID &uuid);

        void RemoveActorByUUID(const UUID &uuid);
        // --------------------------------

        // ---------- COMPONENTS ----------
        template <typename T, typename... Args>
        T *AddComponent(Args... args)
        {
            T *c = new T(args...);
            c->Owner = this;

            if (state == Started)
                c->Start();

            components.push_back(c);
            return c;
        };

        template <typename T>
        T *GetComponent()
        {
            for (Component *c : components)
            {
                T *typedComponent = dynamic_cast<T *>(c);
                if (typedComponent)
                    return typedComponent;
            }

            return nullptr;
        };

        template <typename T>
        std::vector<T *> GetComponents()
        {
            std::vector<T *> v;
            for (Component *c : components)
                v.push_back((T *)c);

            return v;
        };

        template <typename T>
        bool HasComponent()
        {
            return GetComponent<T>() != nullptr;
        };

        template <typename T>
        void RemoveComponent()
        {
            for (auto it = components.begin(); it != components.end(); ++it)
            {
                T *typedComponent = dynamic_cast<T *>(*it);
                if (typedComponent)
                {
                    components.erase(it);
                    delete typedComponent;
                    break;
                }
            }
        };

        template <typename T>
        void RemoveComponent(int index)
        {
            int currentIndex = -1;
            for (auto it = components.begin(); it != components.end(); ++it)
            {
                T *typedComponent = dynamic_cast<T *>(*it);
                if (typedComponent)
                {
                    currentIndex++;
                    if (index == currentIndex)
                    {
                        components.erase(it);
                        delete typedComponent;
                        break;
                    }
                }
            }
        };
    };

    // --------------------------------

}