#include "RigidBody.h"
#include "Core/Logger.h"
#include "Scene/Actor.h"
#include "Math/Math.h"

#include "btBulletDynamicsCommon.h"

namespace Core
{
    static btQuaternion BtQuatFromRot(const Vector3 &rot)
    {
        btQuaternion q; // x and y are swapped due to yaw and pitch
        q.setEulerZYX(rot.z * CE_DEG_TO_RAD, rot.y * CE_DEG_TO_RAD, rot.x * CE_DEG_TO_RAD);
        return q;
    }

    static btTransform ComposeTransform(const Vector3 &pos, const Vector3 &rot)
    {
        btTransform t;
        btVector3 bPos = {pos.x, pos.y, pos.z};
        btQuaternion bQuat = BtQuatFromRot(rot);

        t.setIdentity();
        t.setOrigin(bPos);
        t.setRotation(bQuat);

        return t;
    }

    // extremely inefficient but so far with only one kind of collider its fine
    // todo: maybe not full copy?
    static inline btCollisionShape *_ComposeBtShapeFromCollider(Collider *collider)
    {
        CE_VERIFY_RET(collider, new btBoxShape(btVector3(1, 1, 1)));

        btCollisionShape *ret;
        switch (collider->Type)
        {
        case ColliderType::None:
        default:
            CE_CORE_WARN("Cannot compose Bullet shape from collider, unknown/None collider type. Resorting to a box shape.");
            ret = new btBoxShape(btVector3(1, 1, 1));
            break;

        case ColliderType::Box:
        {
            auto cast = (BoxCollider *)collider;
            ret = new btBoxShape(btVector3(cast->Width, cast->Height, cast->Depth));
        }
        break;
        }

        return ret;
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
        // todo: know what else is to delete for btWorld?
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

    void RigidBody::SetTransform(const Transform &newTransform)
    {
        CE_VERIFY(handle);

        btTransform transform = ComposeTransform(newTransform.Position, newTransform.Rotation);
        SetHandleTransform(transform);
        UpdateTransform();
    }

    // todo: refactor all of this code, preferably in the PhysicsBody base class
    void RigidBody::SetPosition(const Vector3 &pos)
    {
        CE_VERIFY(handle);

        btTransform transform = ComposeTransform(pos, owner->GetTransform()->Rotation);
        SetHandleTransform(transform);
        UpdateTransform();
    }

    void RigidBody::SetRotation(const Vector3 &rot)
    {
        CE_VERIFY(handle);

        btTransform transform = ComposeTransform(owner->GetTransform()->Position, rot);

        SetHandleTransform(transform);
        UpdateTransform();
    }

    void RigidBody::SetAngularVelocity(const Vector3 &vec)
    {
        CE_VERIFY(handle);

        handle->setAngularVelocity(BtVec3FromCeVec3(vec * CE_DEG_TO_RAD));
    }

    void RigidBody::UpdateTransform()
    {
        CE_VERIFY(owner);
        CE_VERIFY(handle);

        btTransform btT = handle->getWorldTransform();
        btScalar yaw, pitch, roll;
        btT.getRotation().getEulerZYX(yaw, pitch, roll);

        auto transform = owner->GetTransform();
        transform->Position.x = handle->getWorldTransform().getOrigin().getX();
        transform->Position.y = handle->getWorldTransform().getOrigin().getY();
        transform->Position.z = handle->getWorldTransform().getOrigin().getZ();

        transform->Rotation.x = roll * CE_RAD_TO_DEG;
        transform->Rotation.y = pitch * CE_RAD_TO_DEG;
        transform->Rotation.z = yaw * CE_RAD_TO_DEG;
    }

    void RigidBody::SetHandleTransform(const btTransform &t)
    {
        CE_VERIFY(handle);

        handle->setWorldTransform(t);
        handle->getMotionState()->setWorldTransform(t);
        handle->setCenterOfMassTransform(t);
    }

    void RigidBodyConfiguration::From(RigidBodyConfiguration *c)
    {
        CE_VERIFY(c);

        Owner = c->Owner;
        LinearDamp = c->LinearDamp;
        AngularDamp = c->AngularDamp;
        Mass = c->Mass;
    }
}