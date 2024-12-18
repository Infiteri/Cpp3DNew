#pragma once

#include "PhysicsBody.h"
#include "Math/Vectors.h"
#include "Math/Matrix4.h"
#include "Math/Quaternion.h"

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
        Vector3 forceAccum;

        /// @brief MUST BE IN RADIANS
        Vector3 torque;

        /// @brief MUST BE IN RADIANS
        Vector3 torqueAccum;

        Vector3 lastFrameAcceleration;

        void _ClearForces();
        void _CalculateData();

        friend class PhysicsEngine;
        friend class Contact;

    public:
        RigidBody();
        ~RigidBody();

        inline RigidBodyConfiguration &GetConfiguration() { return config; };

        void AddVelocity(const Vector3 &v);
        void AddRotation(const Vector3 &v);

        void UseConfiguration(RigidBodyConfiguration *config);

        void Update();

        inline PhysMatrix4x3 &GetTransformMatrix() { return transformMatrix; };

        /// @brief Adds a velocity force to the rigid body.
        /// @param force The force to add.
        void AddForce(const Vector3 &force);

        void AddForceAtPoint(const Vector3 &force, const Vector3 &point);

        /// @brief Adds a rotation force to the rigid body, in degrees. (The conversion happens in this function so no need to do it manually)
        /// @param force The force to add.
        void AddTorque(const Vector3 &force);

        /// @brief TODO NEEDED?
        /// @return
        inline Vector3 &GetVelocity() { return velocity; };

        inline Vector3 GetLastFrameAcceleration() { return lastFrameAcceleration; };

        inline float GetMass() { return config.Mass; };

        inline float GetInverseMass() { return 1.0f / config.Mass; };

        inline Vector3 GetTorque() { return torque; };
    };
}