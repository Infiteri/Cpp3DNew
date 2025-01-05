#include "Collider.h"

namespace Core
{
    Collider::Collider()
    {
        Type = ColliderType::None;
    }

    BoxCollider::BoxCollider()
    {
        Type = ColliderType::Box;
    }
}