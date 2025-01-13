#pragma once

#include "Base.h"
#include "Core/Logger.h"
#include "Math/Math.h"
#include "Math/Vectors.h"
#include "Math/Quaternion.h"
#include "../Collider.h" // fym physics not found ???
#include "btBulletDynamicsCommon.h"

namespace Core
{
    class Actor;
    class CE_API PhysicsBody
    {
    public:
        enum Type
        {
            None,
            Rigid,
            Static,
            Kinematic
        };

    protected:
        Type type;
        Actor *owner;

        friend class Scene;
        friend class PhysicsEngine;

        inline btVector3 BtVec3FromCeVec3(const Vector3 &vec)
        {
            btVector3 v;
            v.setX(vec.x);
            v.setY(vec.y);
            v.setZ(vec.z);
            return v;
        };

        inline btQuaternion BtQuatFromRot(const Vector3 &rot)
        {
            btQuaternion q;
            q.setEulerZYX(rot.z * CE_DEG_TO_RAD, rot.y * CE_DEG_TO_RAD, rot.x * CE_DEG_TO_RAD);
            return q;
        }

        inline Vector3 RotFromBtQuat(const btQuaternion &quat)
        {
            btScalar roll, pitch, yaw; // Rotation around Z, Y, and X axes respectively
            quat.getEulerZYX(roll, yaw, pitch);

            return Vector3(pitch * CE_RAD_TO_DEG,
                           yaw * CE_RAD_TO_DEG,
                           roll * CE_RAD_TO_DEG);
        }

        inline float NormalizeAngle(float angle)
        {
            while (angle > 180.0f)
                angle -= 360.0f;
            while (angle < -180.0f)
                angle += 360.0f;
            return angle;
        }

        inline btTransform ComposeTransform(const Vector3 &pos, const Vector3 &rot)
        {
            btTransform t;
            btVector3 bPos = {pos.x, pos.y, pos.z};
            btQuaternion bQuat = BtQuatFromRot(rot);

            t.setIdentity();
            t.setOrigin(bPos);
            t.setRotation(bQuat);

            return t;
        }

        inline btCollisionShape *_ComposeBtShapeFromCollider(Collider *collider)
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

        void UpdateTransformOwner(btTransform &t);

    public:
        PhysicsBody();
        virtual ~PhysicsBody();

        inline Type GetType() { return type; };
        inline Actor *GetOwner() { return owner; };

        virtual void Update();
        virtual void UseConfiguration(void *config);
        virtual void AddForce(const Vector3 &force) {};

        template <typename T>
        T *As() { return (T *)this; };
    };
}