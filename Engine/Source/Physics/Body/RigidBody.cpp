#include "RigidBody.h"
#include "Core/Logger.h"
#include "Scene/Actor.h"
#include "Math/Math.h"
#include "btBulletDynamicsCommon.h"

namespace Core
{
    void RigidBodyConfiguration::From(RigidBodyConfiguration *c)
    {
        CE_VERIFY(c);

        Owner = c->Owner;
        LinearDamp = c->LinearDamp;
        AngularDamp = c->AngularDamp;
        Mass = c->Mass;
    }

    RigidBody::RigidBody()
    {
        owner = nullptr;
        handle = nullptr;

        config = RigidBodyConfiguration();
        config.ColliderInstance = nullptr;

        type = Rigid;
    }

    RigidBody::~RigidBody()
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

    void RigidBody::UseConfiguration(RigidBodyConfiguration *config)
    {
        this->owner = config->Owner;
        this->config.From(config);

        // Create the collision shape
        btCollisionShape *shape = _ComposeBtShapeFromCollider(config->ColliderInstance);
        btTransform transform = ComposeTransform(owner->GetTransform()->Position, owner->GetTransform()->Rotation);
        btDefaultMotionState *motion = new btDefaultMotionState(transform);

        btVector3 inertia(0, 0, 0);
        if (config->Mass > 0.0f)
            shape->calculateLocalInertia(config->Mass, inertia);

        btRigidBody::btRigidBodyConstructionInfo info(config->Mass, motion, shape, inertia);

        handle = new btRigidBody(info);
        handle->setDamping(config->LinearDamp, config->AngularDamp);
        handle->activate(true);
    }

    void RigidBody::Update()
    {
        UpdateTransform();
    }

    void RigidBody::AddForce(const Vector3 &vec)
    {
        btVector3 v;
        v.setX(vec.x);
        v.setY(vec.y);
        v.setZ(vec.z);
        handle->applyImpulse(v, {0, 0, 0});
    }

    void RigidBody::RotateBy(const Vector3 &deltaRotation)
    {
        CE_VERIFY(handle);

        handle->applyTorque(BtVec3FromCeVec3(deltaRotation * CE_DEG_TO_RAD));
    }

    void RigidBody::SetAngularVelocity(const Vector3 &vec)
    {
        CE_VERIFY(handle);

        handle->setAngularVelocity(BtVec3FromCeVec3(vec * CE_DEG_TO_RAD));
    }

    void RigidBody::UpdateTransform()
    {
        CE_VERIFY(handle);
        UpdateTransformOwner(handle->getWorldTransform());
    }

    void RigidBody::SetPosition(const Vector3 &pos)
    {
        CE_VERIFY(handle);

        btTransform transform = ComposeTransform(pos, owner->GetTransform()->Rotation);
        SetHandleTransform(transform);
        UpdateTransform();
    }

    void RigidBody::SetTransform(const Transform &newTransform)
    {
        CE_VERIFY(handle);

        btTransform transform = ComposeTransform(newTransform.Position, newTransform.Rotation);
        handle->setWorldTransform(transform);

        if (handle->getMotionState())
        {
            handle->getMotionState()->setWorldTransform(transform);
        }

        UpdateTransform();
    }

    void RigidBody::SetHandleTransform(const btTransform &t)
    {
        CE_VERIFY(handle);

        handle->setWorldTransform(t);
        if (handle->getMotionState())
        {
            handle->getMotionState()->setWorldTransform(t);
        }
    }
}
