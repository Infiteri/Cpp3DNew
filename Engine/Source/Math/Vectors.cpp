#include "Vectors.h"
#include "Core/Logger.h"

namespace Core
{
    Vector2::Vector2()
    {
        Set(0, 0);
    }

    Vector2::Vector2(float x, float y)
    {
        Set(x, y);
    }

    Vector2::~Vector2()
    {
    }

    void Vector2::Set(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    void Vector2::Set(Vector2 *v)
    {
        Set(v->x, v->y);
    }

    void Vector2::Set(const Vector2 &v)
    {
        Set(v.x, v.y);
    }

    bool Vector2::NotZero() const
    {
        return (x != 0.0f || y != 0.0f);
    }

    Vector3::Vector3()
    {
        Set(0, 0, 0);
    }

    Vector3::Vector3(const Vector3 &v)
    {
        Set(v);
    }

    Vector3::Vector3(float x, float y, float z)
    {
        Set(x, y, z);
    }

    Vector3::~Vector3()
    {
    }

    void Vector3::Set(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void Vector3::Set(Vector3 *v)
    {
        Set(v->x, v->y, v->z);
    }

    void Vector3::Set(const Vector3 &v)
    {
        Set(v.x, v.y, v.z);
    }

    bool Vector3::NotZero() const
    {
        return (x != 0.0f || y != 0.0f || z != 0.0f);
    }

    Vector4::Vector4()
    {
        Set(0, 0, 0, 0);
    }

    Vector4::Vector4(const Vector4 &v)
    {
        Set(v);
    }

    Vector4::Vector4(float x, float y, float z, float w)
    {
        Set(x, y, z, w);
    }

    Vector4::~Vector4()
    {
    }

    void Vector4::Set(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    void Vector4::Set(const Vector4 &v)
    {
        Set(v.x, v.y, v.z, v.w);
    }
}