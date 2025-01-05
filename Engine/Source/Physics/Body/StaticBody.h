#pragma once

#include "Base.h"

#include "PhysicsBody.h"
#include "Math/Vectors.h"
#include "Math/Matrix4.h"
#include "Math/Quaternion.h"

#include "Physics/Collider.h"

#include "btBulletDynamicsCommon.h"

namespace Core
{
    struct StaticBodyConfiguration
    {
        Actor *Owner;

        /// @brief Owned by the Rigid Body which holds this configuration.
        Collider *ColliderInstance;

        float Mass = 1.0f; // todo: remove

        void From(StaticBodyConfiguration *c);
    };

    class CE_API StaticBody : public PhysicsBody
    {
    private:
        StaticBodyConfiguration config;
        friend class PhysicsEngine;
        friend class Contact;

        btRigidBody *handle;

    public:
        StaticBody();
        ~StaticBody();

        inline StaticBodyConfiguration &GetConfiguration() { return config; };

        void UseConfiguration(StaticBodyConfiguration *config);

        void Update();

        inline float GetMass() { return config.Mass; };
        inline float GetInverseMass() { return 1.0f / config.Mass; };

        inline btRigidBody *GetHandle() { return handle; };
    };
}