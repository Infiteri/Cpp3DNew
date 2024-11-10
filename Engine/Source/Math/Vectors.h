#pragma once

#include "Base.h"
#include <math.h>

namespace Core
{
    class CE_API Vector2
    {
    public:
        float x;
        float y;

        Vector2();
        Vector2(float x, float y);
        ~Vector2();

        void Set(float x, float y);
        void Set(Vector2 *v);

        void operator+=(const Vector2 &o)
        {
            x += o.x;
            y += o.y;
        }

        Vector2 operator+(const Vector2 &o) const
        {
            return {x + o.x, y + o.y};
        }

        Vector2 operator-(const Vector2 &o) const
        {
            return {x - o.x, y - o.y};
        }

        Vector2 operator*(float scalar) const
        {
            return {x * scalar, y * scalar};
        }

        Vector2 operator/(float scalar) const
        {
            return {x / scalar, y / scalar};
        }

        bool operator==(const Vector2 &o) const
        {
            return x == o.x && y == o.y;
        }

        bool operator!=(const Vector2 &o) const
        {
            return !(*this == o);
        }

        bool NotZero() const;

        void Normalize()
        {
            float length = sqrtf(x * x + y * y);
            if (length > 0.0f)
            {
                x /= length;
                y /= length;
            }
        }
    };

    class CE_API Vector3
    {
    public:
        float x;
        float y;
        float z;

        Vector3();
        Vector3(float x, float y, float z);
        ~Vector3();

        void Set(float x, float y, float z);
        void Set(Vector3 *v);
        void Set(const Vector3 &v);

        float &operator[](int index)
        {
            if (index == 0)
                return x;
            else if (index == 1)
                return y;
            return z;
        }

        void operator+=(const Vector3 &o)
        {
            x += o.x;
            y += o.y;
            z += o.z;
        }

        void operator*=(const Vector3 &o)
        {
            x *= o.x;
            y *= o.y;
            z *= o.z;
        }

        void operator-=(const Vector3 &o)
        {
            x -= o.x;
            y -= o.y;
            z -= o.z;
        }

        void operator/=(const Vector3 &o)
        {
            x /= o.x;
            y /= o.y;
            z /= o.z;
        }

        void operator+=(float scalar)
        {
            x += scalar;
            y += scalar;
            z += scalar;
        }

        void operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
        }

        void operator-=(float scalar)
        {
            x -= scalar;
            y -= scalar;
            z -= scalar;
        }

        void operator/=(float scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
        }

        Vector3 operator+(const Vector3 &o) const
        {
            return {x + o.x, y + o.y, z + o.z};
        }

        Vector3 operator-(const Vector3 &o) const
        {
            return {x - o.x, y - o.y, z - o.z};
        }

        Vector3 operator*(const Vector3 &o) const
        {
            return {x * o.x, y * o.y, z * o.z};
        }

        Vector3 operator/(const Vector3 &o) const
        {
            return {x / o.x, y / o.y, z / o.z};
        }

        Vector3 operator*(float scalar) const
        {
            return {x * scalar, y * scalar, z * scalar};
        }

        Vector3 operator/(float scalar) const
        {
            return {x / scalar, y / scalar, z / scalar};
        }

        Vector3 operator%(const Vector3 &vector) const
        {
            return Vector3(y * vector.z - z * vector.y,
                           z * vector.x - x * vector.z,
                           x * vector.y - y * vector.x);
        }

        bool operator==(const Vector3 &o) const
        {
            return x == o.x && y == o.y && z == o.z;
        }

        bool operator!=(const Vector3 &o) const
        {
            return !(*this == o);
        }

        float Dot(const Vector3 &other) const
        {
            return x * other.x + y * other.y + z * other.z;
        }

        bool NotZero() const;

        float Magnitude() const
        {
            return sqrtf(x * x + y * y + z * z);
        }

        void Normalize()
        {
            float length = sqrtf(x * x + y * y + z * z);
            if (length > 0.0f)
            {
                x /= length;
                y /= length;
                z /= length;
            }
        }
    };

}