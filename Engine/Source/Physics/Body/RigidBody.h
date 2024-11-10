#pragma once

#include "PhysicsBody.h"
#include "Math/Vectors.h"
#include "Math/Matrix4.h"
#include "Math/Quaternion.h"
#include "Physics/Collider/Collider.h"

namespace Core
{
    struct RigidBodyConfiguration
    {
        Actor *Owner;
        float LinearDamp = 0.1f;
        float AngularDamp = 0.1f;
        float Mass = 1.0f;

        void From(RigidBodyConfiguration *c);
    };

    class CE_API RigidBody : public PhysicsBody
    {
    private:
        RigidBodyConfiguration config;

        Vector3 velocity;
        Vector3 torque;

        Vector3 forceAccum;
        Vector3 torqueAccum;

        Quaternion orientation;
        PhysMatrix4x3 transformMatrix;
        PhysMatrix3 inverseInertiaTensor;
        PhysMatrix3 inverseInertiaTensorWorld;

        AABBCollider collider;

        void _ClearForces();
        void _CalculateData();

        friend class PhysicsEngine;

    public:
        RigidBody();
        ~RigidBody();

        inline RigidBodyConfiguration &GetConfiguration() { return config; };

        void UseConfiguration(RigidBodyConfiguration *config);

        void Update();

        inline PhysMatrix4x3 *GetTransformMatrix() { return &transformMatrix; };

        /// @brief Adds a velocity force to the rigid body.
        /// @param force The force to add.
        void AddForce(const Vector3 &force);

        void AddForceAtPoint(const Vector3 &force, const Vector3 &point);

        /// @brief Adds a rotation force to the rigid body, in degrees. (The conversion happens in this function so no need to do it manually)
        /// @param force The force to add.
        void AddTorque(const Vector3 &force);

        void SetInertiaTensor(const PhysMatrix3 &matrix);
    };
}