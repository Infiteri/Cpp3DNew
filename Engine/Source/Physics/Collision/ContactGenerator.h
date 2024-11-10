#pragma once

#include "Base.h"
#include "Physics/Collider/Collider.h"
#include "Contact.h"

namespace Core
{
    class ContactGenerator
    {
    public:
        ContactGenerator();
        ~ContactGenerator();

        bool BoxAndBox(AABBCollider *a1, AABBCollider *a2);
    };
}