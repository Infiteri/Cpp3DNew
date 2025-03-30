#include "Transform.h"

#include "Math.h"
#include "Quaternion.h"

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
        Rotation.Set(x, y, z);
    }

    void Transform::SetScale(float x, float y, float z)
    {
        Scale.Set(x, y, z);
    }

    Matrix4 Transform::GetTransformMatrix()
    {
        Quaternion quatX{{1, 0, 0}, Rotation.x * CE_DEG_TO_RAD};
        Quaternion quatY{{0, 1, 0}, Rotation.y * CE_DEG_TO_RAD};
        Quaternion quatZ{{0, 0, 1}, Rotation.z * CE_DEG_TO_RAD};
        Quaternion quat = quatX * quatY * quatZ;

        Matrix4 translationMatrix = Matrix4::Translate(&Position);
        Matrix4 scaleMatrix = Matrix4::Scale(&Scale);
        Matrix4 rotationMatrix = quat.GetMatrix();

        return scaleMatrix * (translationMatrix * (quat.GetMatrix()));
    }
}