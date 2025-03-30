#pragma once

#include "PhysicsBody.h"
#include "Math/Vectors.h"
#include "Math/Matrix4.h"
#include "Math/Quaternion.h"
#include "Math/Transform.h"
#include "Physics/Collider.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

namespace Core
{
    struct KinematicBodyConfiguration
    {
        Actor *Owner;

        /// @brief Owned by the Kinematic Body which holds this configuration.
        Collider *ColliderInstance;

        void From(KinematicBodyConfiguration *c);
    };

    class CE_API KinematicBody : public PhysicsBody
    {
    private:
        KinematicBodyConfiguration config;

        btRigidBody *handle;
        btPairCachingGhostObject *ghost;

        friend class PhysicsEngine;

        void _UpdateWithTransform(const btTransform& transform);
        void _CheckCollisions();

    public:
        KinematicBody();
        ~KinematicBody();

        inline KinematicBodyConfiguration &GetConfiguration() { return config; };

        void UseConfiguration(KinematicBodyConfiguration *config);

        void Update();

        void Move(const Vector3 &offset);
        void Rotate(const Vector3 &offset);
    };
}