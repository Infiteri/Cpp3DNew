#pragma once

#include "Base.h"
#include "Math/Vectors.h"

namespace Core
{
    class Actor;
    class CE_API PhysicsBody
    {
    protected:
        Actor *owner;

        friend class Scene;

    public:
        PhysicsBody();
        ~PhysicsBody();

        inline Actor *GetOwner() { return owner; };

        virtual void Update();

        Vector3 &GetPosition();

        template <typename T>
        T *As() { return (T *)this; };
    };
}