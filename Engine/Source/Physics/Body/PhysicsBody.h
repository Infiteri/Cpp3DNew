#pragma once

#include "Base.h"
#include "Math/Vectors.h"
#include "Physics/Collider/Collider.h"

namespace Core
{
    class Actor;
    class CE_API PhysicsBody
    {
    protected:
        Actor *owner;

        AABBCollider collider;

        friend class Scene;

    public:
        PhysicsBody();
        ~PhysicsBody();

        inline Actor *GetOwner() { return owner; };

        virtual void Update();

        Vector3 &GetPosition();
        Vector3 GetRotation(bool asRadians = false);

        inline Collider *GetCollider() { return &collider; };

        template <typename T>
        T *As() { return (T *)this; };
    };
}