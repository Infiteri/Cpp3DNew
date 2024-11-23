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

        Quaternion orientation;
        PhysMatrix4x3 transformMatrix;
        PhysMatrix3 inverseInertiaTensor;
        PhysMatrix3 inverseInertiaTensorWorld;

        Vector3 lastFrameAcceleration;

        void _ClearForces();
        void _CalculateData();

        friend class PhysicsEngine;
        friend class Contact;

        void _AddVelocity(const Vector3 &v);
        void _AddRotation(const Vector3 &v);

    public:
        RigidBody();
        ~RigidBody();

        inline RigidBodyConfiguration &GetConfiguration() { return config; };

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

        void SetInertiaTensor(const PhysMatrix3 &matrix);
        inline PhysMatrix3 GetInverseInertiaTensorWorld() { return inverseInertiaTensorWorld; };
        inline PhysMatrix3 GetInverseInertiaTensor() { return inverseInertiaTensor; };

        /// @brief Will set the orientation and will change the body rotation to the respected values/
        /// @param quat The quaternion.
        void SetOrientation(const Quaternion &quat);

        /// @brief TODO NEEDED?
        /// @return
        inline Vector3 &GetVelocity() { return velocity; };

        inline Quaternion &GetOrientation() { return orientation; };

        inline Vector3 GetLastFrameAcceleration() { return lastFrameAcceleration; };

        inline float GetMass() { return config.Mass; };

        inline float GetInverseMass() { return 1.0f / config.Mass; };

        inline Vector3 GetTorque() { return torque; };
    };
}