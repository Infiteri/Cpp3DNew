#include "KinematicBody.h"
#include "Scene/Actor.h"

namespace Core
{
    void KinematicBodyConfiguration::From(KinematicBodyConfiguration *c)
    {
        CE_VERIFY(c);
        Owner = c->Owner;
    }

    KinematicBody::KinematicBody()
    {
        owner = nullptr;
        handle = nullptr;

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

        if (config.ColliderInstance)
            delete config.ColliderInstance;
    }

    void KinematicBody::UseConfiguration(KinematicBodyConfiguration *config)
    {
        owner = config->Owner;
        this->config.From(config);

        // Create the collision shape
        btCollisionShape *shape = _ComposeBtShapeFromCollider(config->ColliderInstance);
        btTransform transform = ComposeTransform(owner->GetTransform()->Position, owner->GetTransform()->Rotation);
        btDefaultMotionState *motion = new btDefaultMotionState(transform);

        btVector3 inertia(0, 0, 0);

        btRigidBody::btRigidBodyConstructionInfo info(0.0f, motion, shape, inertia);

        handle = new btRigidBody(info);
        handle->setFlags(btRigidBody::CF_KINEMATIC_OBJECT);
        handle->setCollisionFlags(handle->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        handle->setActivationState(DISABLE_DEACTIVATION);
        handle->activate(true);
    }

    void KinematicBody::Update()
    {
        CE_VERIFY(handle);
        UpdateTransformOwner(handle->getWorldTransform());
    }

// todo: clean functions
#pragma region CLEANUP

    void KinematicBody::Move(const Vector3 &offset)
    {
        CE_VERIFY(handle);

        btTransform transform;
        handle->getMotionState()->getWorldTransform(transform);

        btVector3 btOffset = BtVec3FromCeVec3(offset);

        btVector3 currentPosition = transform.getOrigin();
        btVector3 newPosition = currentPosition + btOffset;

        transform.setOrigin(newPosition);
        handle->getMotionState()->setWorldTransform(transform);
        handle->setWorldTransform(transform); // todo: update actor transform ?
    }

    void KinematicBody::Rotate(const Vector3 &offset)
    {
        CE_VERIFY(handle);

        btTransform transform;
        handle->getMotionState()->getWorldTransform(transform);

        btQuaternion currentRotation = transform.getRotation();

        float pitch = offset.x * SIMD_RADS_PER_DEG; // X-axis
        float yaw = offset.y * SIMD_RADS_PER_DEG;   // Y-axis
        float roll = offset.z * SIMD_RADS_PER_DEG;  // Z-axis

        btQuaternion rotationOffset;
        rotationOffset.setEulerZYX(roll, yaw, pitch);

        btQuaternion newRotation = currentRotation * rotationOffset;

        newRotation.normalize();

        transform.setRotation(newRotation);
        handle->getMotionState()->setWorldTransform(transform);
        handle->setWorldTransform(transform);
    }
#pragma endregion
}
