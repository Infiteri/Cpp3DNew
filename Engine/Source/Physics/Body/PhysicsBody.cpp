#include "PhysicsBody.h"
#include "Scene/Actor.h"

namespace Core
{
    static Vector3 ZERO_VECTOR = {0, 0, 0};

    PhysicsBody::PhysicsBody()
    {
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
}