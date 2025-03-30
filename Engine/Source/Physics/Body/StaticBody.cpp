#include "StaticBody.h"
#include "Scene/Actor.h"
#include "btBulletDynamicsCommon.h"

//!
// TODO: CHANGE TO GHOST OBJECTS
//!

// todo: couple shared behavior in PhysicsBody
namespace Core
{
    StaticBody::StaticBody()
    {
        owner = nullptr;
        handle = nullptr;
        type = Static;

        config = StaticBodyConfiguration();
        config.ColliderInstance = nullptr;
    }

    StaticBody::~StaticBody()
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

    void StaticBody::UseConfiguration(StaticBodyConfiguration *config)
    {
        this->owner = config->Owner;
        this->config.From(config);

        btCollisionShape *cubeShape = _ComposeBtShapeFromCollider(config->ColliderInstance);

        btTransform transform = ComposeTransform(owner->GetTransform()->Position, owner->GetTransform()->Rotation);
        btDefaultMotionState *motion = new btDefaultMotionState(transform);

        // massless
        btRigidBody::btRigidBodyConstructionInfo staticBodyInfo(0.0f, motion, cubeShape);
        handle = new btRigidBody(staticBodyInfo);
        handle->setUserPointer(owner);
    }

    void StaticBody::Update()
    {
        if (!owner)
            return;

        auto transform = owner->GetTransform();
        transform->Position.x = handle->getWorldTransform().getOrigin().getX();
        transform->Position.y = handle->getWorldTransform().getOrigin().getY();
        transform->Position.z = handle->getWorldTransform().getOrigin().getZ();

        transform->Rotation.x = handle->getWorldTransform().getRotation().getX() * CE_RAD_TO_DEG;
        transform->Rotation.y = handle->getWorldTransform().getRotation().getY() * CE_RAD_TO_DEG;
        transform->Rotation.z = handle->getWorldTransform().getRotation().getZ() * CE_RAD_TO_DEG;
    }

    void StaticBodyConfiguration::From(StaticBodyConfiguration *c)
    {
        Owner = c->Owner;
        Mass = c->Mass;
    }
}