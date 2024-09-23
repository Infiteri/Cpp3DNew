#pragma once

#include "Base.h"
#include "Vectors.h"
#include "Matrix4.h"

namespace Core
{
    struct CE_API Transform
    {
        Transform();

        Vector3 Position;
        Vector3 Rotation;
        Vector3 Scale;

        void From(Transform* transform);

        Matrix4 GetTransformMatrix();
    };

}