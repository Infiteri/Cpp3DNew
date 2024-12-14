#pragma once

#include "Base.h"
#include "Contact.h"

namespace Core
{
    struct ContactResolver
    {
        std::vector<Contact> Contacts;

        int PositionIterations = 15;
        int VelocityIterations = 15;

        float PositionEpsilon = 0.1f;
        float VelocityEpsilon = 0.1f;

        void Resolve();

        void PrepareContacts();
        void AdjustVelocity();
        void AdjustPosition();
    };

}