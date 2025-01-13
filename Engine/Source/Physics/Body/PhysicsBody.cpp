#include "PhysicsBody.h"
#include "Scene/Actor.h"

namespace Core
{
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

    void PhysicsBody::UpdateTransformOwner(btTransform &t)
    {
        CE_VERIFY(owner);

        auto ot = owner->GetTransform();
        ot->Position.x = t.getOrigin().x();
        ot->Position.y = t.getOrigin().y();
        ot->Position.z = t.getOrigin().z();

        btQuaternion currentRotation = t.getRotation();
        Vector3 euler = RotFromBtQuat(currentRotation);
        ot->Rotation.x = NormalizeAngle(euler.x);
        ot->Rotation.y = NormalizeAngle(euler.y);
        ot->Rotation.z = NormalizeAngle(euler.z);
    }
}