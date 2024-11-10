#pragma once

#include "Base.h"

#include "Math/Vectors.h"
#include "Math/Matrix4.h"

namespace Core
{
    class Collider
    {
    public:
        enum Type
        {
            Base,
            Box
        };

    protected:
        Type type = Base;

    public:
        PhysMatrix4x3 *TransformMatrix = nullptr;

        Collider() {};
        ~Collider() {};

        inline Type GetType() { return type; };

        template <typename T>
        T *As() { return (T *)this; };

        Vector3 GetAxis(int index);
    };

    class AABBCollider : public Collider
    {
    public:
        Vector3 Size;

        AABBCollider() { type = Box; };
        ~AABBCollider() {}
    };
}