#pragma once

#include "Base.h"
#include "Math/Vectors.h" 

#include <vector>

namespace Core
{
    struct Contact
    {
        Vector3 ContactPoint;
        Vector3 ContactNormal;
        float Penetration = 0.0f;
    };

    struct ContactData
    {
        std::vector<Contact> Contacts;
    };
}