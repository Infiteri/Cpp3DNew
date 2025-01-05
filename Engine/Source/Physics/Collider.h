#pragma once

#include "Base.h"

namespace Core
{
    enum class ColliderType
    {
        None = 0,
        Box
    };

    struct Collider
    {
        ColliderType Type;
        Collider();
    };

    struct BoxCollider : public Collider
    {
        BoxCollider();

        float Width = 1.0f;
        float Height = 1.0f;
        float Depth = 1.0f;
    };
}