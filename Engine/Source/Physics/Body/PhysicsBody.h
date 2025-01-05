#pragma once

#include "Base.h"
#include "Core/Logger.h"
#include "Math/Vectors.h"
#include "Math/Quaternion.h"
#include "btBulletDynamicsCommon.h"

namespace Core
{
    class Actor;
    class CE_API PhysicsBody
    {
    public:
        enum Type
        {
            None,
            Rigid,
            Static
        };

    protected:
        Type type;
        Actor *owner;

        friend class Scene;
        friend class PhysicsEngine;

        btVector3 BtVec3FromCeVec3(const Vector3 &vec)
        {
            btVector3 v;
            v.setX(vec.x);
            v.setY(vec.y);
            v.setZ(vec.z);
            return v;
        };

    public:
        PhysicsBody();
        virtual ~PhysicsBody();

        inline Type GetType() { return type; };
        inline Actor *GetOwner() { return owner; };

        virtual void Update();
        virtual void UseConfiguration(void *config);
        virtual void AddForce(const Vector3 &force) {};

        template <typename T>
        T *As() { return (T *)this; };
    };
}