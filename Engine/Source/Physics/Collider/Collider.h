#pragma once

#include "Base.h"

#include "Math/Vectors.h"
#include "Math/Matrix4.h"

namespace Core
{
    class PhysicsBody;
    class CE_API Collider
    {
    public:
        enum Type
        {
            None,
            Box
        };

    protected:
        Type type = None;

    public:
        PhysicsBody *Owner;
        PhysMatrix4x3 *TransformMatrix = nullptr;

        Collider();
        ~Collider() {};

        virtual void From(Collider *col) {};

        inline Type GetType() { return type; };

        template <typename T>
        T *As() { return (T *)this; };

        Vector3 GetAxis(int index);
    };

    class CE_API AABBCollider : public Collider
    {
    public:
        Vector3 Size;

        AABBCollider();
        ~AABBCollider() {}
        void From(AABBCollider *col);
    };
}