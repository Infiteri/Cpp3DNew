#pragma once

#include "Base.h"
#include "Physics/Collider/Collider.h"
#include "Contact.h"

namespace Core
{
    class CollisionDetector
    {
    public:
        CollisionDetector() {};
        ~CollisionDetector() {};

        bool BoxAndBox(AABBCollider *a1, AABBCollider *a2, Contact *contact);
    };
}