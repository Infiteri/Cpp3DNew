#pragma once

#include "PhysicsBody.h"
#include "Math/Vectors.h"
#include "Math/Matrix4.h"
#include "Math/Quaternion.h"
#include "Math/Transform.h"
#include "Physics/Collider.h"

#include "btBulletDynamicsCommon.h"

namespace Core
{
    struct RigidBodyConfiguration
    {
        Actor *Owner;

        /// @brief Owned by the Rigid Body which holds this configuration.
        Collider *ColliderInstance;

        float LinearDamp = 0.1f;
        float AngularDamp = 0.1f;
        float Friction = 0.5f;
        float Restitution = 0.5f;
        float Mass = 1.0f;

        void From(RigidBodyConfiguration *c);
    };

    class CE_API RigidBody : public PhysicsBody
    {
    private:
        RigidBodyConfiguration config;

        btRigidBody *handle;

        friend class PhysicsEngine;

        void SetHandleTransform(const btTransform& t);
        void UpdateTransform();

    public:
        RigidBody();
        ~RigidBody();

        inline RigidBodyConfiguration &GetConfiguration() { return config; };

        void UseConfiguration(RigidBodyConfiguration *config);

        void Update();

        inline float GetMass() { return config.Mass; };
        inline float GetInverseMass() { return 1.0f / config.Mass; };

        void AddForce(const Vector3 &vec);

        inline btRigidBody *GetHandle() { return handle; };

        void SetTransform(const Transform &newTransform);

        void SetPosition(const Vector3 &pos);
        void SetRotation(const Vector3 &rot);

        void RotateBy(const Vector3& deltaRotation);

        void SetAngularVelocity(const Vector3& vec);
    };
}