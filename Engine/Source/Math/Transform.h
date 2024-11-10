#pragma once

#include "Base.h"
#include "Vectors.h"
#include "Matrix4.h"

namespace Core
{
    struct CE_API Transform
    {
        Transform();
        Transform(Transform &t);

        /// @brief The transform position of the body.
        Vector3 Position;

        /// @brief The transform rotation of the body. In degrees.
        Vector3 Rotation;

        /// @brief The scale of the body.
        Vector3 Scale;

        void From(Transform *transform);

        void SetPosition(const Vector3 &v);
        void SetRotation(const Vector3 &v);
        void SetScale(const Vector3 &v);

        void SetPosition(float x, float y, float z);
        void SetRotation(float x, float y, float z);
        void SetScale(float x, float y, float z);

        Matrix4 GetTransformMatrix();
    };

}