#pragma once

#include "Base.h"

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
    };

}