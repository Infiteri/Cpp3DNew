#include "StaticBody.h"
#include "Scene/Actor.h"
#include "btBulletDynamicsCommon.h"

// todo: couple shared behavior in PhysicsBody
namespace Core
{
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

        btTransform transform;
        btQuaternion quat;
        transform.setIdentity();
        auto &tPos = owner->GetTransform()->Position;
        auto &tRot = owner->GetTransform()->Rotation;
        transform.setOrigin(btVector3(tPos.x, tPos.y, tPos.z));
        quat.setEuler(tRot.x * CE_DEG_TO_RAD, tRot.y * CE_DEG_TO_RAD, tRot.z * CE_DEG_TO_RAD);
        transform.setRotation(quat);

        btDefaultMotionState *motion = new btDefaultMotionState(transform);

        // massless
        btRigidBody::btRigidBodyConstructionInfo staticBodyInfo(0.0f, motion, cubeShape);
        handle = new btRigidBody(staticBodyInfo);
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