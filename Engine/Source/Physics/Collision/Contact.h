#pragma once

#include "Base.h"
#include "Math/Vectors.h"
#include "Physics/Body/PhysicsBody.h"

#include <vector>

namespace Core
{
    struct Contact
    {
        Contact() = default;
        Contact(const Contact &c);

        union
        {
            struct
            {
                PhysicsBody *One;
                PhysicsBody *Two;
            };

            PhysicsBody *Body[2];
        };

        PhysicsBody *BodyAt(CeU32 index)
        {
            if (index == 0)
                return One;
            return Two;
        }

        bool BodySame(PhysicsBody *a, PhysicsBody *b);

        PhysMatrix3 ContactToWorld;

        float Friction;
        float Restitution;
        float Penetration = 0.0f;
        float DesiredDeltaVelocity;

        Vector3 ContactPoint;
        Vector3 ContactNormal;
        Vector3 RelativeContactPosition[2];
        Vector3 ContactVelocity;

        void CalculateInternalData();

        /// @brief Computes an orthonormal basis for the contact.
        /// This is stored as a 3x3 matrix.
        /// The x direction is generated from the contact normal.
        /// The y and z directions are set so they are at right angles to it.
        void CalculateContactBasis();

        Vector3 CalculateLocalVelocity(PhysicsBody *body, int index);
        void CalculateDesiredDeltaVelocity();

        void ApplyPositionChange(Vector3 linearChange[2],
                                 Vector3 angularChange[2],
                                 float penetration);

        void ApplyVelocityChange(Vector3 velocityChange[2],
                                 Vector3 rotationChange[2]);

        Vector3 CalculateFrictionlessImpulse(PhysMatrix3 *inverseInertiaTensor);
        Vector3 CalculateFrictionImpulse(PhysMatrix3 *inverseInertiaTensor);
    };
}