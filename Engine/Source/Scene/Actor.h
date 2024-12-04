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
    class Scene;

    typedef std::vector<Actor *> ActorList;

    class CE_API Actor
    {
    public:
        enum State
        {
            // CAN RENDER
            Initialized,

            // CAN RENDER && UPDATE
            Started,

            // CAN RENDER && UPDATE
            Running,

            // CAN RENDER
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
        Scene *scene;

        friend class Scene;
        friend class SceneSerializer;
        friend class ComponentSerializer;

    public:
        Actor();
        ~Actor();

        /// @brief Will return a new copy of an actor, all of its components are also copied respectively.
        /// @param a The actor co copy.
        /// @param copyUUID Wether or not to use the same UUID for the new actor. True by default.
        /// @return A pointer to a new actor.
        static Actor *From(Actor *a, bool copyUUID = true);

        void SetName(const std::string &name) { this->name = name; };
        inline std::string GetName() { return name; };

        inline State GetState() { return state; };
        inline Scene *GetScene() { return scene; };

        inline UUID GetUUID() { return UUID(id.Get()); };

        void Start();
        void Render();
        void Update();
        void Stop();

        /// @brief Will return the world matrix of the object.
        /// @return A new transform object.
        Transform GetWorldTransform();

        inline Transform *GetTransform() { return &transform; };
        inline Matrix4 GetTransformMatrix() { return worldMatrix; };
        inline Matrix4 GetLocalMatrix() { return localMatrix; };

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
            {
                T *typedComponent = dynamic_cast<T *>(c);
                if (typedComponent)
                    v.push_back(typedComponent);
            }

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
        void RemoveComponents()
        {
            for (auto it = components.begin(); it != components.end(); ++it)
            {
                T *typedComponent = dynamic_cast<T *>(*it);
                if (typedComponent)
                {
                    components.erase(it);
                    delete typedComponent;
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