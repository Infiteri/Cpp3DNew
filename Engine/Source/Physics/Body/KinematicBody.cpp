#include "KinematicBody.h"
#include "Scene/Actor.h"
#include "Physics/PhysicsEngine.h"

namespace Core
{
    void KinematicBodyConfiguration::From(KinematicBodyConfiguration *c)
    {
        CE_VERIFY(c);
        Owner = c->Owner;
    }

    void KinematicBody::_UpdateWithTransform(const btTransform &transform)
    {
        CE_VERIFY(ghost);
        ghost->setWorldTransform(transform);

        CE_VERIFY(handle);
        handle->setWorldTransform(transform);

        CE_VERIFY(handle->getMotionState());
        handle->getMotionState()->setWorldTransform(transform);
    }

    void KinematicBody::_CheckCollisions()
    {
        CE_VERIFY(ghost);

        int numOverlapping = ghost->getNumOverlappingObjects();
        for (int i = 0; i < numOverlapping; i++)
        {
            const btCollisionObject *other = ghost->getOverlappingObject(i);
            Actor *otherActor = (Actor *)other->getUserPointer();

            if (owner->GetUUID().Get() == otherActor->GetUUID().Get())
            {
                continue;
            }

            if (otherActor)
            {
                PhysicsEngine::_OnCollision(owner, otherActor);
            }
        }
    }

    KinematicBody::KinematicBody()
    {
        owner = nullptr;
        handle = nullptr;
        ghost = nullptr;

        config = KinematicBodyConfiguration();
        config.ColliderInstance = nullptr;

        type = Kinematic;
    }

    KinematicBody::~KinematicBody()
    {
        if (handle)
        {
            delete handle->getMotionState();
            delete handle->getCollisionShape();
            delete handle;
        }

        if (ghost)
        {
            delete ghost;
            ghost = nullptr;
        }

        if (config.ColliderInstance)
            delete config.ColliderInstance;
    }

    void KinematicBody::UseConfiguration(KinematicBodyConfiguration *config)
    {
        owner = config->Owner;
        this->config.From(config);

        btCollisionShape *shape = _ComposeBtShapeFromCollider(config->ColliderInstance);
        btTransform transform = ComposeTransform(owner->GetTransform()->Position, owner->GetTransform()->Rotation);
        btDefaultMotionState *motion = new btDefaultMotionState(transform);
        btRigidBody::btRigidBodyConstructionInfo info(0.0f, motion, shape);

        ghost = new btPairCachingGhostObject();
        ghost->setCollisionShape(shape);
        ghost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
        ghost->setWorldTransform(transform);
        ghost->setUserPointer(owner);

        handle = new btRigidBody(info);
        handle->setFlags(btRigidBody::CF_KINEMATIC_OBJECT);
        handle->setCollisionFlags(handle->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        handle->setActivationState(DISABLE_DEACTIVATION);
        handle->activate(true);
        handle->setUserPointer(owner);
    }

    void KinematicBody::Update()
    {
        CE_VERIFY(handle);
        UpdateTransformOwner(handle->getWorldTransform());
        ghost->setWorldTransform(handle->getWorldTransform());
        _CheckCollisions();
    }

    void KinematicBody::Move(const Vector3 &offset)
    {
        CE_VERIFY(ghost);

        btTransform transform = ghost->getWorldTransform();

        btVector3 btOffset = BtVec3FromCeVec3(offset);
        btVector3 currentPosition = transform.getOrigin();
        btVector3 newPosition = currentPosition + btOffset;

        transform.setOrigin(newPosition);
        _UpdateWithTransform(transform);
    }

    void KinematicBody::Rotate(const Vector3 &offset)
    {
        CE_VERIFY(ghost);

        btTransform transform = ghost->getWorldTransform();
        btQuaternion currentRotation = transform.getRotation();

        float pitch = offset.x * SIMD_RADS_PER_DEG; // X-axis
        float yaw = offset.y * SIMD_RADS_PER_DEG;   // Y-axis
        float roll = offset.z * SIMD_RADS_PER_DEG;  // Z-axis

        btQuaternion rotationOffset;
        rotationOffset.setEulerZYX(roll, yaw, pitch);

        btQuaternion newRotation = currentRotation * rotationOffset;
        newRotation.normalize();

        transform.setRotation(newRotation);
        _UpdateWithTransform(transform);
    }
}
