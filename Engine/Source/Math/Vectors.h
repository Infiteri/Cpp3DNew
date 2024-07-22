#pragma once

#include "Base.h"
#include "Math.h"

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
            float length = Core::Math::Sqrt(x * x + y * y);
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

        void operator+=(const Vector3 &o)
        {
            x += o.x;
            y += o.y;
            z += o.z;
        }

        Vector3 operator+(const Vector3 &o) const
        {
            return {x + o.x, y + o.y, z + o.z};
        }

        Vector3 operator-(const Vector3 &o) const
        {
            return {x - o.x, y - o.y, z - o.z};
        }

        Vector3 operator*(float scalar) const
        {
            return {x * scalar, y * scalar, z * scalar};
        }

        Vector3 operator/(float scalar) const
        {
            return {x / scalar, y / scalar, z / scalar};
        }

        bool operator==(const Vector3 &o) const
        {
            return x == o.x && y == o.y && z == o.z;
        }

        bool operator!=(const Vector3 &o) const
        {
            return !(*this == o);
        }

        bool NotZero() const;

        void Normalize()
        {
            float length = Core::Math::Sqrt(x * x + y * y + z * z);
            if (length > 0.0f)
            {
                x /= length;
                y /= length;
                z /= length;
            }
        }
    };

}