#include "PhysicsBody.h"
#include "Scene/Actor.h"

namespace Core
{
    static Vector3 ZERO_VECTOR = {0, 0, 0};
    PhysicsBody::PhysicsBody()
    {
    }

    PhysicsBody::~PhysicsBody()
    {
    }

    void PhysicsBody::Update()
    {
    }
    Vector3 &PhysicsBody::GetPosition()
    {
        ZERO_VECTOR.Set(0, 0, 0);
        if (!owner)
            return ZERO_VECTOR;

        return owner->GetTransform()->Position;
    }
}