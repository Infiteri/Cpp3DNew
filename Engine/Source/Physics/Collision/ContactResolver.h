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

        /// @brief Least amount of penetration required to solve an impulse.
        float PositionEpsilon = 0.1f;

        /// @brief Least amount of desired delta velocity required to solve an impulse.
        float VelocityEpsilon = 0.1f;

        void Resolve();

        void PrepareContacts();
        void AdjustVelocity();
        void AdjustPosition();
    };

}