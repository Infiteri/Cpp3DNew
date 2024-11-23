#pragma once

#include "Base.h"
#include "Contact.h"

namespace Core
{
    struct ContactResolver
    {
        std::vector<Contact> Contacts;

        int PositionIterations = 5;
        int VelocityIterations = 5;

        float PositionEpsilon = 0.001f;
        float VelocityEpsilon = 0.001f;

        void Resolve();

        void PrepareContacts();
        void AdjustVelocity();
        void AdjustPosition();
    };

}