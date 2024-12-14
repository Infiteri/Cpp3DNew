#include "PhysicsBody.h"
#include "Scene/Actor.h"

namespace Core
{
    static Vector3 ZERO_VECTOR = {0, 0, 0};

    PhysicsBody::PhysicsBody()
    {
        collider = nullptr;
        type = None;
    }

    PhysicsBody::~PhysicsBody()
    {
    }

    void PhysicsBody::Update()
    {
    }

    void PhysicsBody::UseConfiguration(void *config)
    {
    }

    Vector3 &PhysicsBody::GetPosition()
    {
        ZERO_VECTOR.Set(0, 0, 0);
        if (!owner)
            return ZERO_VECTOR;

        return owner->GetTransform()->Position;
    }

    Vector3 PhysicsBody::GetRotation(bool asRadians)
    {
        ZERO_VECTOR.Set(0, 0, 0);
        if (!owner)
            return ZERO_VECTOR;

        return owner->GetTransform()->Rotation * (asRadians ? CE_DEG_TO_RAD : 1.0f);
    }

    void PhysicsBody::SetCollider(Collider *newCollider)
    {
        if (collider)
            delete collider;
        collider = newCollider;
    }

    void PhysicsBody::SetOrientation(const Quaternion &quat)
    {
        orientation.Set(quat);
        orientation.Normalize();
    }

    void PhysicsBody::SetInertiaTensor(const PhysMatrix3 &matrix)
    {
        inverseInertiaTensor.SetInverse(matrix);
    }
}