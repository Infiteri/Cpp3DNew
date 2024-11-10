#include "Transform.h"

#include "Math/Math.h"

namespace Core
{
    Transform::Transform()
    {
        Position.Set(0, 0, 0);
        Rotation.Set(0, 0, 0);
        Scale.Set(1, 1, 1);
    }

    Transform::Transform(Transform &t)
    {
        Position.Set(&t.Position);
        Rotation.Set(&t.Rotation);
        Scale.Set(&t.Scale);
    }

    void Transform::From(Transform *transform)
    {
        Position.Set(&transform->Position);
        Rotation.Set(&transform->Rotation);
        Scale.Set(&transform->Scale);
    }

    void Transform::SetPosition(const Vector3 &v)
    {
        Position.Set(v);
    }

    void Transform::SetRotation(const Vector3 &v)
    {
        Rotation.Set(v);
    }

    void Transform::SetScale(const Vector3 &v)
    {
        Scale.Set(v);
    }

    void Transform::SetPosition(float x, float y, float z)
    {
        Position.Set(x, y, z);
    }

    void Transform::SetRotation(float x, float y, float z)
    {
        Rotation.Set(x,y,z);
    }

    void Transform::SetScale(float x, float y, float z)
    {
        Scale.Set(x,y,z);
    }

    Matrix4 Transform::GetTransformMatrix()
    {
        Matrix4 pos = Matrix4::Translate(&Position);
        Matrix4 rot = Matrix4::RotationXYZ(Rotation * CE_DEG_TO_RAD);
        Matrix4 scale = Matrix4::Scale(&Scale);

        return (pos * rot) * scale;
    }
}