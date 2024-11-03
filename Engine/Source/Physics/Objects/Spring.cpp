#include "Spring.h"
#include "Math/Vectors.h"
#include "Math/Math.h"

namespace Core
{
    void Spring::Update()
    {
        if (!TargetBody || !OtherSpring || !OtherSpring->TargetBody)
            return;

        Vector3 force;
        force = TargetBody->GetPosition();
        force -= OtherSpring->TargetBody->GetPosition();

        // Calculate the magnitude of the force.
        float magnitude = force.Magnitude();
        magnitude = Math::Abs(magnitude - RestLength);
        magnitude *= SpringConstant;
        // Calculate the final force and apply it.
        force.Normalize();
        force *= -magnitude;
        TargetBody->AddForce(force);
    }
}