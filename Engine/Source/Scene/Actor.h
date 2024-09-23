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

    public:
        Actor();
        ~Actor();

        void SetName(const std::string &name) { this->name = name; };
        inline std::string GetName() { return name; };

        inline State GetState() { return state; };

        UUID GetUUID() { return UUID(id); };

        void Start();
        void Render();
        void Update();
        void Stop();

        inline Transform *GetTransform() { return &transform; };
        inline Matrix4 GetTransformMatrix() { return worldMatrix; };

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
    };

    // --------------------------------

}