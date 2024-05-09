#include "Vectors.h"

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

    bool Vector2::NotZero() const
    {
        return (x != 0.0f || y != 0.0f);
    }

}