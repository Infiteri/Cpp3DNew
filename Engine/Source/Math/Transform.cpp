#include "Transform.h"

namespace Core
{
    Transform::Transform()
    {
        Position.Set(0, 0, 0);
        Rotation.Set(0, 0, 0);
        Scale.Set(1, 1, 1);
    }

    void Transform::From(Transform *transform)
    {
        Position.Set(&transform->Position);
        Rotation.Set(&transform->Rotation);
        Scale.Set(&transform->Scale);
    }

    Matrix4 Transform::GetTransformMatrix()
    {
        Matrix4 pos = Matrix4::Translate(&Position);
        Matrix4 rot = Matrix4::RotationZYX(Rotation);
        Matrix4 scale = Matrix4::Scale(&Scale);

        return (pos * rot) * scale;
    }
}